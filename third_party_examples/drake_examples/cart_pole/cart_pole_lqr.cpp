// This example shows how to use LQR controller for cart-pole by linearizing the system around its
// upper fixed point. This means the controller won't be able to stablize the system to the upper
// fixed if the system starts with pole underneath.

#include <experimental/filesystem>
#include <memory>
#include <string>

#include <gflags/gflags.h>

#include "drake/common/drake_assert.h"
#include "drake/common/find_resource.h"
#include "drake/geometry/drake_visualizer.h"
#include "drake/geometry/scene_graph.h"
#include "drake/lcm/drake_lcm.h"
#include "drake/multibody/parsing/parser.h"
#include "drake/multibody/plant/multibody_plant.h"
#include "drake/multibody/tree/prismatic_joint.h"
#include "drake/multibody/tree/revolute_joint.h"
#include "drake/systems/analysis/simulator.h"
#include "drake/systems/controllers/linear_quadratic_regulator.h"
#include "drake/systems/framework/diagram_builder.h"
#include "drake/systems/primitives/constant_vector_source.h"

namespace drake {
namespace examples {
namespace multibody {
namespace cart_pole {
namespace {

namespace fs = std::experimental::filesystem;

using geometry::SceneGraph;
using lcm::DrakeLcm;

// "multibody" namespace is ambiguous here without "drake::".
using drake::multibody::MultibodyPlant;
using drake::multibody::Parser;
using drake::multibody::PrismaticJoint;
using drake::multibody::RevoluteJoint;

DEFINE_string(model, "../cart_pole/cart_pole.sdf", "Path to acrobot.sdf file.");

DEFINE_double(target_realtime_rate,
              1.0,
              "Desired rate relative to real time.  See documentation for "
              "Simulator::set_target_realtime_rate() for details.");

DEFINE_double(simulation_time, 10.0, "Desired duration of the simulation in seconds.");

DEFINE_double(time_step,
              0,
              "If greater than zero, the cart_pole is modeled as a system with "
              "discrete updates and period equal to this time_step. "
              "If 0, the cart_pole is modeled as a continuous system.");

DEFINE_double(Kp, 10.0, "Kp");
DEFINE_double(Ki, 0.1, "Ki");
DEFINE_double(Kd, 5.0, "Kd");

#include "drake/multibody/parsing/parser.h"
#include "drake/multibody/plant/multibody_plant.h"
#include "drake/multibody/tree/prismatic_joint.h"
#include "drake/multibody/tree/revolute_joint.h"
#include "drake/multibody/tree/uniform_gravity_field_element.h"
#include "drake/systems/controllers/linear_quadratic_regulator.h"

using namespace drake;

using drake::multibody::MultibodyPlant;
using drake::multibody::Parser;
using drake::multibody::PrismaticJoint;
using drake::multibody::RevoluteJoint;
using drake::multibody::UniformGravityFieldElement;
using systems::Context;
using systems::InputPort;

std::unique_ptr<systems::AffineSystem<double>> makeCartPoleLQRController(
    const std::string cart_pole_filename,
    const std::string slider_joint_name = "CartSlider",
    const std::string pin_joint_name = "PolePin",
    const double target_translation = 0.0,
    const double target_translation_rate = 0.0,
    const double target_angle = M_PI,
    const double target_angular_rate = 0.0) {
  MultibodyPlant<double> cart_pole(0.0);
  Parser parser(&cart_pole);
  parser.AddModelFromFile(cart_pole_filename);
  cart_pole.Finalize();

  std::unique_ptr<Context<double>> context = cart_pole.CreateDefaultContext();

  const InputPort<double>& actuation_port = cart_pole.get_actuation_input_port();
  actuation_port.FixValue(context.get(), 0.0);

  // Get joints so that we can set initial conditions.
  const PrismaticJoint<double>& cart_slider =
      cart_pole.GetJointByName<PrismaticJoint>(slider_joint_name);
  const RevoluteJoint<double>& pole_pin = cart_pole.GetJointByName<RevoluteJoint>(pin_joint_name);

  // Set target state
  cart_slider.set_translation(context.get(), target_translation);
  cart_slider.set_translation_rate(context.get(), target_translation_rate);
  pole_pin.set_angle(context.get(), target_angle);
  pole_pin.set_angular_rate(context.get(), target_angular_rate);

  Eigen::Matrix4d Q = Eigen::Matrix4d::Identity();
  Q(0, 0) = 100;  // cart position weight
  Q(1, 1) = 100;  // pole position weight
  Q(2, 2) = 1;    // cart velocity weight
  Q(3, 3) = 1;    // pole velocity weight
  Eigen::MatrixXd R = Eigen::MatrixXd::Identity(1, 1) * 0.001;
  return systems::controllers::LinearQuadraticRegulator(
      cart_pole, *context, Q, R,
      Eigen::Matrix<double, 0, 0>::Zero() /* No cross state/control costs */,
      actuation_port.get_index());
}

int do_main() {
  systems::DiagramBuilder<double> builder;

  SceneGraph<double>& scene_graph = *builder.AddSystem<SceneGraph>();
  scene_graph.set_name("scene_graph");

  // Make and add the cart_pole model.
  const std::string model_path = fs::canonical(FLAGS_model);
  MultibodyPlant<double>& cart_pole = *builder.AddSystem<MultibodyPlant>(FLAGS_time_step);
  cart_pole.set_name("cart_pole");
  cart_pole.RegisterAsSourceForSceneGraph(&scene_graph);
  Parser(&cart_pole, &scene_graph).AddModelFromFile(model_path);

  // Now the model is complete.
  cart_pole.Finalize();

  auto lqr = builder.AddSystem(makeCartPoleLQRController(model_path));

  builder.Connect(cart_pole.get_state_output_port(), lqr->get_input_port());
  builder.Connect(lqr->get_output_port(), cart_pole.get_actuation_input_port());

  // Connect plant with scene_graph to get collision information.
  DRAKE_DEMAND(!!cart_pole.get_source_id());
  // Sanity check on the availability of the optional source id before using it.
  DRAKE_DEMAND(cart_pole.geometry_source_is_registered());

  builder.Connect(cart_pole.get_geometry_poses_output_port(),
                  scene_graph.get_source_pose_port(cart_pole.get_source_id().value()));

  geometry::DrakeVisualizer<double>::AddToBuilder(&builder, scene_graph);
  auto diagram = builder.Build();

  // Create a context for this system:
  std::unique_ptr<systems::Context<double>> diagram_context = diagram->CreateDefaultContext();
  diagram->SetDefaultContext(diagram_context.get());
  systems::Context<double>& cart_pole_context =
      diagram->GetMutableSubsystemContext(cart_pole, diagram_context.get());

  // Get joints so that we can set initial conditions.
  const PrismaticJoint<double>& cart_slider =
      cart_pole.GetJointByName<PrismaticJoint>("CartSlider");
  const RevoluteJoint<double>& pole_pin = cart_pole.GetJointByName<RevoluteJoint>("PolePin");

  // Set initial state.
  cart_slider.set_translation(&cart_pole_context, 0.0);
  pole_pin.set_angle(&cart_pole_context, M_PI + 0.4);

  systems::Simulator<double> simulator(*diagram, std::move(diagram_context));

  simulator.set_publish_every_time_step(false);
  simulator.set_target_realtime_rate(FLAGS_target_realtime_rate);
  simulator.Initialize();
  simulator.AdvanceTo(FLAGS_simulation_time);

  return 0;
}

}  // namespace
}  // namespace cart_pole
}  // namespace multibody
}  // namespace examples
}  // namespace drake

int main(int argc, char* argv[]) {
  gflags::SetUsageMessage(
      "A simple cart pole demo using Drake's MultibodyPlant,"
      "with SceneGraph visualization. "
      "Launch drake-visualizer before running this example.");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return drake::examples::multibody::cart_pole::do_main();
}

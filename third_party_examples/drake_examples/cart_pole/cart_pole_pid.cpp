// This example shows how to use PID controller for cart-pole.
// Failed to tune the parameter of PID to achieve desired positions of both the cart and the
// pole at the same time.

#include <experimental/filesystem>
#include <iostream>
#include <memory>
#include <string>

#include <gflags/gflags.h>

#include <drake/geometry/meshcat_visualizer.h>
#include "drake/common/drake_assert.h"
#include "drake/common/find_resource.h"
#include "drake/geometry/scene_graph.h"
#include "drake/lcm/drake_lcm.h"
#include "drake/multibody/parsing/parser.h"
#include "drake/multibody/plant/multibody_plant.h"
#include "drake/multibody/tree/prismatic_joint.h"
#include "drake/multibody/tree/revolute_joint.h"
#include "drake/systems/analysis/simulator.h"
#include "drake/systems/controllers/pid_controlled_system.h"
#include "drake/systems/framework/diagram_builder.h"
#include "drake/systems/primitives/constant_vector_source.h"

// sudo apt install libgraphviz-dev
#include <graphviz/gvc.h>
#include <fstream>

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

DEFINE_double(KpPole, 10.0, "KpPole");
DEFINE_double(KiPole, 0.1, "KiPole");
DEFINE_double(KdPole, 5.0, "KdPole");

DEFINE_double(KpCart, 10.0, "KpCart");
DEFINE_double(KiCart, 0.1, "KiCart");
DEFINE_double(KdCart, 5.0, "KdCart");

#include "drake/multibody/parsing/parser.h"
#include "drake/multibody/plant/multibody_plant.h"
#include "drake/multibody/tree/prismatic_joint.h"
#include "drake/multibody/tree/revolute_joint.h"
#include "drake/multibody/tree/uniform_gravity_field_element.h"

using namespace drake;

using drake::multibody::MultibodyPlant;
using drake::multibody::Parser;
using drake::multibody::PrismaticJoint;
using drake::multibody::RevoluteJoint;
using drake::multibody::UniformGravityFieldElement;
using systems::Context;
using systems::InputPort;

bool createPNGFromDotFile(std::string dot_file_name) {
  GVC_t* gvc;
  Agraph_t* g;
  FILE* fp;
  gvc = gvContext();
  fp = fopen((dot_file_name + ".dot").c_str(), "r");
  g = agread(fp, 0);
  gvLayout(gvc, g, "dot");
  gvRender(gvc, g, "png", fopen((dot_file_name + ".png").c_str(), "w"));
  gvFreeLayout(gvc, g);
  agclose(g);
  return (gvFreeContext(gvc));
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

  // The choices of PidController constants here are fairly arbitrary,
  // but seem to effectively swing up the pendulum and hold it.
  Eigen::VectorXd kp = (Eigen::VectorXd(2) << FLAGS_KpCart, FLAGS_KpPole).finished();
  Eigen::VectorXd ki = (Eigen::VectorXd(2) << FLAGS_KiCart, FLAGS_KiPole).finished();
  Eigen::VectorXd kd = (Eigen::VectorXd(2) << FLAGS_KdCart, FLAGS_KdPole).finished();

  std::cout << "cart pole's actuation input size: " << cart_pole.get_actuation_input_port().size()
            << "\n";
  std::cout << "cart pole's output port size: " << cart_pole.get_state_output_port().size() << "\n";

  Eigen::MatrixXd px(4, 4);
  px.setIdentity();
  Eigen::MatrixXd py(1, 2);
  py.setOnes();
  auto pid_controller =
      builder.AddSystem<systems::controllers::PidController<double>>(px, py, kp, ki, kd);

  // TODO WIP:
  // https://deepnote.com/@jeh15/Example-Drake-Cart-pole-with-PID-22940130-02da-4410-8a60-4d5974597472
  // Check state_projection and output_projection of PidController

  // Constant Desired State Trajectory Block:
  Eigen::VectorXd target_state = (Eigen::VectorXd(4) << 0, M_PI, 0, 0).finished();
  auto desired_trajectory = builder.AddSystem<systems::ConstantVectorSource<double>>(target_state);

  builder.Connect(desired_trajectory->get_output_port(),
                  pid_controller->get_input_port_desired_state());
  builder.Connect(cart_pole.get_state_output_port(),
                  pid_controller->get_input_port_estimated_state());

  std::cout << "pid output control size: " << pid_controller->get_output_port_control().size()
            << "\n";
  builder.Connect(pid_controller->get_output_port_control(), cart_pole.get_actuation_input_port());

  // Connect plant with scene_graph to get collision information.
  DRAKE_DEMAND(!!cart_pole.get_source_id());
  // Sanity check on the availability of the optional source id before using it.
  DRAKE_DEMAND(cart_pole.geometry_source_is_registered());

  builder.Connect(cart_pole.get_geometry_poses_output_port(),
                  scene_graph.get_source_pose_port(cart_pole.get_source_id().value()));

  auto meshcat = std::make_shared<drake::geometry::Meshcat>(8080);
  geometry::MeshcatVisualizer<double>::AddToBuilder(&builder, scene_graph, meshcat);
  auto diagram = builder.Build();

  std::ofstream out("graph.dot");
  out << diagram->GetGraphvizString(2);
  out.close();
  createPNGFromDotFile("graph");

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
  std::cout << "Simulation is ready. Press enter to continue ...";
  std::cin.ignore();
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

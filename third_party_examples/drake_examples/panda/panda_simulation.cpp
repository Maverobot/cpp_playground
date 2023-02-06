/// @file
///
/// Implements a simulation of the KUKA iiwa arm.  Like the driver for the
/// physical arm, this simulation communicates over LCM using lcmt_iiwa_status
/// and lcmt_iiwa_command messages. It is intended to be a be a direct
/// replacement for the KUKA iiwa driver and the actual robot hardware.

#include <experimental/filesystem>
#include <memory>

#include <gflags/gflags.h>

#include <drake/common/drake_assert.h>
#include <drake/geometry/drake_visualizer.h>
#include <drake/geometry/scene_graph.h>
#include <drake/multibody/parsing/parser.h>
#include <drake/systems/analysis/simulator.h>
#include <drake/systems/framework/diagram.h>
#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/lcm/lcm_interface_system.h>

#include <limits>
#include <string>

namespace fs = std::experimental::filesystem;

DEFINE_double(simulation_sec,
              std::numeric_limits<double>::infinity(),
              "Number of seconds to simulate.");
DEFINE_string(urdf, "", "Name of urdf to load");
DEFINE_double(target_realtime_rate,
              1.0,
              "Playback speed.  See documentation for "
              "Simulator::set_target_realtime_rate() for details.");
DEFINE_bool(torque_control, false, "Simulate using torque control mode.");
DEFINE_double(sim_dt,
              3e-3,
              "The time step to use for MultibodyPlant model "
              "discretization.");

namespace drake {
namespace examples {
namespace panda {
namespace {

int DoMain() {
  systems::DiagramBuilder<double> builder;

  // Adds a plant.
  auto [plant, scene_graph] = multibody::AddMultibodyPlantSceneGraph(&builder, FLAGS_sim_dt);
  const char* kModelPath = "../panda/robots/panda_arm.urdf";
  const std::string urdf = (!FLAGS_urdf.empty() ? FLAGS_urdf : fs::canonical(kModelPath).string());
  auto iiwa_instance = multibody::Parser(&plant, &scene_graph).AddModelFromFile(urdf);
  plant.WeldFrames(plant.world_frame(), plant.GetFrameByName("world"));
  plant.Finalize();

  // Creates and adds LCM publisher for visualization.
  auto lcm = builder.AddSystem<systems::lcm::LcmInterfaceSystem>();
  geometry::DrakeVisualizerd::AddToBuilder(&builder, scene_graph, lcm);

  // Since we welded the model to the world above, the only remaining joints
  // should be those in the arm.

  auto sys = builder.Build();

  systems::Simulator<double> simulator(*sys);

  simulator.set_publish_every_time_step(false);
  simulator.set_target_realtime_rate(FLAGS_target_realtime_rate);
  simulator.Initialize();

  // Simulate for a very long time.
  simulator.AdvanceTo(FLAGS_simulation_sec);

  return 0;
}

}  // namespace
}  // namespace panda
}  // namespace examples
}  // namespace drake

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return drake::examples::panda::DoMain();
}

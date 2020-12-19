#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>

// Example of custom SyncActionNode (synchronous action)
// without ports.
class ApproachObject : public BT::SyncActionNode {
 public:
  ApproachObject(const std::string& name) : BT::SyncActionNode(name, {}) {}

  // You must override the virtual function tick()
  BT::NodeStatus tick() override {
    std::cout << "ApproachObject: " << this->name() << std::endl;
    return BT::NodeStatus::SUCCESS;
  }
};

// Simple function that return a NodeStatus
BT::NodeStatus CheckBattery() {
  std::cout << "[ Battery: OK ]" << std::endl;
  return BT::NodeStatus::SUCCESS;
}

// We want to wrap into an ActionNode the methods open() and close()
class GripperInterface {
 public:
  GripperInterface() : _open(true) {}

  BT::NodeStatus open() {
    _open = true;
    std::cout << "GripperInterface::open" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

  BT::NodeStatus close() {
    std::cout << "GripperInterface::close" << std::endl;
    _open = false;
    return BT::NodeStatus::SUCCESS;
  }

 private:
  bool _open;  // shared information
};

// Simple function that return a NodeStatus
auto DoorOpen = [](BT::TreeNode&) {
  std::cout << "[ Door: open ]" << std::endl;
  return BT::NodeStatus::SUCCESS;
};

int main(int argc, char* argv[]) {
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<ApproachObject>("ApproachObject");
  factory.registerSimpleCondition("CheckBattery", std::bind(CheckBattery));
  factory.registerSimpleCondition("DoorOpen", DoorOpen);

  // You can also create SimpleActionNodes using methods of a class
  GripperInterface gripper;
  factory.registerSimpleAction("OpenGripper", std::bind(&GripperInterface::open, &gripper));
  factory.registerSimpleAction("CloseGripper", std::bind(&GripperInterface::close, &gripper));

  // clang-format off
  static const char* xml_text = R"(
    <root main_tree_to_execute = "MainTree" >
        <BehaviorTree ID="MainTree">
           <Sequence name="root_sequence">
               <CheckBattery   name="battery_ok"/>
               <Sequence name="grasp_if_door_is_open">
                   <DoorOpen       name="door_open"/>
                   <Sequence name="grasp">
                       <OpenGripper    name="open_gripper"/>
                       <ApproachObject name="approach_object"/>
                       <CloseGripper   name="close_gripper"/>
                   </Sequence>
               </Sequence>
           </Sequence>
        </BehaviorTree>
    </root>
  )";
  // clang-format on

  auto tree = factory.createTreeFromText(xml_text);

  tree.tickRoot();

  return 0;
}

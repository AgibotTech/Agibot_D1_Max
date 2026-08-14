# SDK Motion State Transitions

**Note: When transitioning from locked state to other states (stand up, crawl, lie down), it is considered unlocked.**

*When a stand up command is issued, the stand up state is reported during the standing process. After standing is complete, it automatically switches to the universal state, in-place state, or stair-climbing state based on the current mode.*

![Image](images/SDK-State.png)

## SDK Mode Switching

**When switching modes, the robot will automatically stand up; after standing is complete, the corresponding state is reported.**

## SDK Command Delivery

The figure shows two command delivery designs.

![Image](images/SDK-SendCmd.png)

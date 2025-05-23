using Script.Dynamic;
using Script.Engine;
using Script.CoreUObject;

namespace Script.CoreUObject
{
    [UClass]
    public partial class UDynamicActorComponent : UActorComponent
    {
        public UDynamicActorComponent()
        {
        }

        [Override]
        public override void ReceiveBeginPlay()
        {
            base.ReceiveBeginPlay();
        }

        [Override]
        public override void ReceiveEndPlay(EEndPlayReason EndPlayReason)
        {
            base.ReceiveEndPlay(EndPlayReason);
        }
    }
}
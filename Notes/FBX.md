http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_CC93340E_C4A1_49EE_B048_E898F856CFBF_htm

Objects in the FBX SDK are always created in the **right handed, Y-Up** axis system

The implicit coordinate system is in **centimeters**.

FbxAxisSystem::ConvertScene()

FbxSystemUnit::ConvertScene() 

Note that calls to ConvertScene() do not change the vertex values of meshes, and only affect node transforms and animations.



Unity Quaternion.eulerAngles z->x->y

A rotation that rotates euler.z degrees around the z axis, euler.x degrees around the x axis, and euler.y degrees around the y axis (in that order).
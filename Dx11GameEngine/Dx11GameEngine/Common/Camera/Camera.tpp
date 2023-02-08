#include "Core/GraphicEngine/GraphicEngine.h"

Camera<T>::Camera(CameraPositioner::ICameraPositioner* camPositioner,const T& initialData)
{
	p_positioner = camPositioner;

}
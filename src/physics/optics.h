/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_PHYSICS_OPTICS_H_
#define ORION_PHYSICS_OPTICS_H_
#include <orion.h>
namespace orion {

	// ��Ե�巴����
	Float frDielectric(Float cosThetaI, Float etaI, Float etaT);

	// ���巴����
	Spectrum frConductor(Float cosThetaI, const Spectrum &etai, const Spectrum &etat, const Spectrum &k);

}

#endif // !ORION_PHYSICS_OPTICS_H_

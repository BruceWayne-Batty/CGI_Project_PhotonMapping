# Blog for DH2323 VT23 Computer Graphics and Interaction Final Project: Photon Mapping Using Cornell Box
#### 5.12 Updated

Currently a skeleton of photon emission and tracing is built. The light source is a point light source beneath the center of the roof. 5000 photons are emitted in random direction. Only reflection and diffusion are considered for now, each with 50% probability to happen. The maximum numbers of iterations of both reflection and diffusion are 10. A result is shown below.

![screenshot](https://github.com/BruceWayne-Batty/CGI_Project_PhotonMapping/assets/128699437/5da9b2e1-aea6-4f19-9a69-5f2b9e1c3142)

Currently, storing photons vector to a kDTree gives errors. The test code of kDTree is commented in skeletonSDL2.cpp.



#### 5.18 Updated

Fixed the issue with storing photons vector in the kDTree. By implementing the subscript operator overloading for the Photon class, it now allows direct access to the position attributes （`position.x`, `position.y`, `position.z`）of a Photon object through subscript indices.
# CGI_Project_PhotonMapping
Repository for CGI project

#### 5.12 Yuxuan Updated

Currently a skeleton of photon emission and tracing is built. The light source is a point light source beneath the center of the roof. 5000 photons are emitted in random direction. Only reflection and diffusion are considered for now, each with 50% probability to happen. The maximum numbers of iterations of both reflection and diffusion are 10. A result is shown below.

![screenshot](https://github.com/BruceWayne-Batty/CGI_Project_PhotonMapping/assets/128699437/5da9b2e1-aea6-4f19-9a69-5f2b9e1c3142)


Currently, storing photons vector to a kDTree gives errors. The test code of kDTree is commented in skeletonSDL2.cpp.



A useful link: https://github.com/zsyzgu/new_photon_mapping/tree/master



（我想我们可以开始着手做illumination部分，在做illumination时如果发现需要新的参数，可以持续修改Photon class。最后可能需要给TestModel中的triangles添加material，从而应用refraction，尝试caustic效果。）

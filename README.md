# deepstream-facenet-landmarks-app

A Deepstream based implementation of Facedetector and Landmarks extractor as Primary+Secondary Gie


* [Prerequisites](#prereqs)
* [Running Sample Applications](#run_sample)
* [Pipeline](#pipeline)
* [MetaData](#metadata)

<a name="prereqs"></a>
## Prerequisites

* Ubuntu 20.04/18
* [DeepStream SDK 6.1.0](https://developer.nvidia.com/deepstream-download) or later
* Nvidia GPU

<a name="run_sample"></a>
## Running Sample Applications
```
git clone deepstream-facenet-landmarks-app
cd deepstream-facenet-landmarks-app/apps/deepstream-app
# check the Makefile Paths
make -j

cd ../launchConfig
./run.sh <..apps/deepstream-app/deepstream-app -c deepstream_app_int8.txt>
```

<a name="pipeline"></a>
# Pipeline
This app uses nvidia tlt "facenet" model for facedetection and "fpenet" facelandmark detection model but embedded in Deepstream-SDK app. 
Both models are Int8 models for Fast Inferencing.
I got more than 1000fps in both model in T4.

```
Input Image -> streammux -> primary-gie (face) -> Secondary-gie (landmarks) -> output.
```

<a name="metadata"></a>
## Metadata
"nvds_add_facemark_meta" API is used to extract the face landmarks and attach the metadata to object meta. Later it is attached to frame_meta.
* process_meta in deepstream-app.cpp, that integrates the secondary gie output to primary gie pipeline
* sample_faciallandmarks_config.txt , that is used for preprocessing settings for secondary gie.



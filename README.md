# Urhox

Urhox is a library that extend Urho3D using some externals libraries and custom modules.

Urhox support next features :
- ImGui
- Spark particles engine

## Screenshots
<img src="https://i.imgur.com/02vazdM.png" width="50%" height="%"><img src="https://i.imgur.com/Xng4U3k.png" width="50%" height="%">

## Building

Use cmake and URHO3D_HOME variable to the Urho3D build directory.

example :

```cpp
mkdir BUILD
cd BUILD
cmake .. -DURHO3D_HOME=../../Urho3D/BUILD
make -j12
```

Out library, samples and public headers are generated into the "output" directory.

## Prepare samples data

- Copy Urho3D "Data" and "CoreData" to output/Bin
- Copy bin/SparkData to output/Bin


# TangentFix
Fix TANGENT use AverageNormal algorithm.

Works for both GI,HI3,HSR,ZZZ mods.

![contrast](https://github.com/StarBobis/TangentFix/assets/151726114/006a8908-c35e-4a6c-8ba9-e8eec60002b4)

# How to use?
Drag your mod's Position.buf on this program, that is all.

![Usage Tutorial](https://github.com/StarBobis/TangentFix/assets/151726114/42c4968a-d67a-4309-aee6-39f34eae3445)

Three additional tips if you are making new mod instead of fix old mod's TANGENT:

- hoyo's model use AutoSmooth 89° for normals ,you can set it in blender before you export mod,then this algorithm will works better.
- hoyo's model split some point on some sharp edges to get correct normal, if you can do it as like hoyo's model, this algorithm will works better.
- if you merge your model's point by distance before you export, this algorithm will works better.

# Acknowledgement

Special thanks for GIMI's developer: SilentNightSound

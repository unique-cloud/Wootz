# Wootz
## A compiler that generate Tensorflow code from prototxt

Token Language definiation: Please see file "Token Language"

Grammar definiation: Please see file "Grammar"

Test cases: "alexnet_scale.prototxt", "inception_v1.prototxt", "inception_v2.prototxt"

Generated code: "alexnet_scale.py", "alexnet_scale_multiplexing.py"

...

How to run the code?

`
make
./main "inception_v1.prototxt"
`

Requirement packages?

No. Just need g++ to compile.

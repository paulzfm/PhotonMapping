# if [[ $# -ne 1 ]]; then
#     echo "Usage: $0 <scene file>"
#     exit 1
# fi

./rt

ppmtojpeg test.ppm > test.jpeg
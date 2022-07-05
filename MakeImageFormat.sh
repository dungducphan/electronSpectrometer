#!/bin/bash

rm ImageFormat_*.root

root -b -q "ROOT2Img.C(0)"
root -b -q "ROOT2Img.C(1)"
root -b -q "ROOT2Img.C(2)"
root -b -q "ROOT2Img.C(3)"

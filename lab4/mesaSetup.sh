#!/bin/bash
echo "Setting up Mesa3D"
module purge
module load gcc/9.2.0
module load cmake/3
module load xalt
module use /storage/home/hcocice1/shared-classes/modules/lmod/linux-rhel7-x86_64/
module load Core/mesa Core/mesa-glu
module list


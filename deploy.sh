#! /bin/bash
rmmod pru_rproc
modprobe pru_rproc
make clean
make
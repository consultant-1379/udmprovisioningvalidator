#!/bin/bash
#Script to run perf test on udmprovisioningvalidator uservice
#Run with the location of injector tool as first argument

INJECTOR_DIR=$1

#Step 1: compile with debug info and prepare uservice needs
mason.py -build -target debug -no-sonar-report

CURRENT_DIR=$(pwd)
export VALIDATORPORT=9002 OAISCHEMAFILE=$CURRENT_DIR/../../schema/UDMProvisioningValidation.yaml LOGCONTROLPATH=$CURRENT_DIR/udmlog.json

#Step 2: perf configurations
sudo sysctl -w kernel.perf_event_paranoid=-1
sudo sysctl -w kernel.kptr_restrict=0

#Step 3: run perf along with uservice
cd ../../.debug/bin
perf record -g -s ./UdmProvisioningValidator.usvc.elf &

#Step 4: Inject traffic
cd $INJECTOR_DIR
udm=$(./injector.usvc.elf -u 'http://127.0.0.1:9002/validation/v1/validate/validate' -m POST -f 2000 -d 15 -j $CURRENT_DIR/udmprovdata.json  -M 10)

#step 5: store injector results
cd $CURRENT_DIR
echo "$udm" > udmresults.txt

#kill perf process
kill -INT $(ps -u | grep linux-tools | awk '{print $2}')
kill -INT $(ps -u | grep ./UdmProvisioningValidator.usvc.elf | awk '{print $2}')

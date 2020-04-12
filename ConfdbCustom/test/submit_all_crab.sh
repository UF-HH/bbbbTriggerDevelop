echo ".... launching 2018 menu ...."

TAG="fix2018"

for DSET in \
/EphemeralHLTPhysics1/Run2018D-v1/RAW \
/EphemeralHLTPhysics2/Run2018D-v1/RAW \
/EphemeralHLTPhysics3/Run2018D-v1/RAW \
/EphemeralHLTPhysics4/Run2018D-v1/RAW \
/EphemeralHLTPhysics5/Run2018D-v1/RAW \
/EphemeralHLTPhysics6/Run2018D-v1/RAW \
/EphemeralHLTPhysics8/Run2018D-v1/RAW ; do \

REQNAME=${DSET}
REQNAME=`echo "${DSET}" | tr - _`
REQNAME=${REQNAME#"/"}
REQNAME=${REQNAME%"/RAW"}
REQNAME=`echo ${REQNAME} | sed 's/\//_/'`
REQNAME=${REQNAME}_${TAG}
echo $REQNAME

crab submit -c crabconfig_.py \
    General.requestName=${REQNAME} \
    Data.outputDatasetTag=${REQNAME} \
    Data.inputDataset=${DSET} \
    Data.lumiMask="Json/json_2018D_Ephemeral_20181022.txt" \
    JobType.psetName="hlt_wbtag.py"
done
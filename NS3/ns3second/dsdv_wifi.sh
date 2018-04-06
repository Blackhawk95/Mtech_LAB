echo "starting.."

echo "removing old files"
rm unsat.csv
rm sat.csv

echo "unsaturated network"
for number in {1..5}
do
echo "$number node network.."	
./waf --run "scratch/dsdvwifi_0_1 --saturated=false --nwifisource=$number --filename=unsat.csv" > /dev/null
done

echo "saturated network"
for number in {1..5}
do
echo "$number node network.."	
./waf --run "scratch/dsdvwifi_0_1 --saturated=true --nwifisource=$number --filename=sat.csv" > /dev/null
done

python dsdv_wifi_viewer.py sat.csv &
python dsdv_wifi_viewer.py unsat.csv &



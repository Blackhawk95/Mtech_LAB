class Activity:
	def __init__(self,start_time,end_time):
		self.start_time=start_time
		self.end_time=end_time
	


activity=[]
activity.append(Activity(1,4))
activity.append(Activity(3,5))
activity.append(Activity(0,6))
activity.append(Activity(5,7))
activity.append(Activity(3,8))
activity.append(Activity(5,9))
activity.append(Activity(6,10))
activity.append(Activity(8,11))
activity.append(Activity(8,12))
activity.append(Activity(2,13))
activity.append(Activity(12,14))
print("Input Activities\n-----------------------------\nStart Time  End Time")
for act in activity:
	print act.start_time,"\t\t",act.end_time
max_compatible_activity=[]
activity.sort(key=lambda x:x.start_time)
print("\nInput Activities after sorting \n-------------------------\n Start Time  End Time")
for act in activity:
	print act.start_time,"\t\t",act.end_time
for i in range(0,len(activity)):	
	compatible_activity=[]
	k=i
	compatible_activity.append(activity[i])
	for j in range(i+1,len(activity)):
		if(activity[j].start_time>=activity[k].end_time):
			compatible_activity.append(activity[j])
			k=j
	if len(compatible_activity)>len(max_compatible_activity):
		max_compatible_activity=compatible_activity
print("\nMaximum Compatible Activities \n-------------------------\n Start Time  End Time")
for act in max_compatible_activity:
	print act.start_time,"\t\t",act.end_time


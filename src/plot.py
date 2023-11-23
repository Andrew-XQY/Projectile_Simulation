import collections
import matplotlib.pyplot as plt
import os, glob, csv
from statistics import mean, stdev

# control output
fire_rate = {'musket' : 15, 'rifle' : 10}
color = {'musket' : 'green', 'rifle' : 'blue'}
plt.style.use('ggplot') # ggplot, seaborn, seaborn-dark-palette
resolution = 300
output_type = 'pdf' # png, pdf

# change to working directory
script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)

def get_all_files(path, type='txt'):
    search_path = os.path.join(path, '*.{}'.format(type))
    files = glob.glob(search_path)
    return files


def get_statistics(nums, name):
    mean_val = mean(nums)
    std_dev = round(stdev(nums), 4)
    print(f"Weapon: {name}, number of trials: {len(nums)} | Mean: {mean_val}, Standard Deviation: {std_dev}")
    # what percentage of trials is the soldier hitting the target at least 50% of the time
    count = len([i for i in numbers if i > fire_rate[name]//2])
    print(f"Percentage of trials hitting the target at least 50% of the time: {100*count/len(nums)}%\n", "-"*50)
    

'''
plot histograms of the number of hits in different trials
'''  
txt_files = sorted(get_all_files("../output/"))

for dir in txt_files:
    name = dir.split("/")[-1].split(".")[0]
    
    with open(dir, 'r') as f:
        numbers = [int(line.strip()) for line in f]
        
    get_statistics(numbers, name.split("_")[0])
    counter = collections.Counter(numbers)
    counts = list(counter.values())
    values = list(counter.keys())
    
    plt.grid(True)
    plt.gca().set_axisbelow(True)
    plt.bar(values, counts, color=color[name.split("_")[0]])
    plt.autoscale()
    plt.xlabel(f'Number of hits in a trial ({name.split("_")[0]})')
    plt.ylabel('Number of trials')
    plt.savefig(f'../output/histogram_{name}.{output_type}', dpi=resolution)
    plt.clf()


'''
plot the mean and standard deviation of the number of hits vs. distance
'''
csv_files = sorted(get_all_files("../output/", type='csv'))

for dir in csv_files:
    distance, mean_val, std_dev = [], [], []
    with open(dir, 'r') as file:
        csv_reader = csv.reader(file)
        next(csv_reader)
        for row in csv_reader:
            distance.append(float(row[0]))  
            mean_val.append(float(row[1]))      
            std_dev.append(float(row[2]))   
        
    name = dir.split("/")[-1].split(".")[0]    
    plt.errorbar(distance, mean_val, yerr=std_dev, fmt='o', ecolor='red', capsize=5, 
                 markersize=2,label='Standard Deviation')
    plt.xlabel('Distance (m)')
    plt.ylabel('Mean')
    plt.title(f'Mean and Standard Deviation vs. Distance ({name.split("_")[0]})')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'../output/distance_relation_{name}.{output_type}', dpi=resolution)
    plt.clf()


'''
find the distance where two weapons have the same performance (mean) of hitting the target 
'''
mean_val_musket = []
with open(csv_files[0], 'r') as file:  # csv_files[0] is musket
    csv_reader = csv.reader(file)
    next(csv_reader)
    for row in csv_reader:
        mean_val_musket.append(float(row[1]))  

min_distance = float('inf')
index = 0
for i in range(len(mean_val)):
    a, b = mean_val[i], mean_val_musket[i]
    if abs(a - b) < min_distance:
        min_distance = abs(a - b)
        index = i
            
plt.plot(distance, mean_val_musket, label='Musket')
plt.plot(distance, mean_val, label='Rifle')
plt.scatter(distance[index], mean_val[index], color='red', zorder=5, label='Intersection')
plt.annotate(f'({distance[index]}, {mean_val[index]})', (distance[index] + 0.5, mean_val[index] + 0.5))
plt.xlabel('Distance (m)')
plt.ylabel('Mean value of number of hits')
plt.title('Distance vs. Mean value of number of hits')
plt.legend()
plt.savefig(f'../output/distance_relation_comparision.{output_type}', dpi=resolution)
plt.clf()

print(f"At distance {distance[index]}m, the two weapons have the similar performance (mean) of hitting the target.")


'''
additional part: plot the angle
'''
angle_files = sorted(get_all_files("../output/angle/", type='txt'))

for dir in angle_files:
    name = dir.split("/")[-1].split(".")[0]
    with open(dir, 'r') as f:
        data = [float(line.strip()) for line in f]
    num_bins = 200 # the number of bins
    plt.hist(data, num_bins, facecolor=color[name.split("_")[0]], alpha=0.5)
    plt.xlabel('Fire angle (degree) hits the target')
    plt.ylabel('Frequency')
    plt.grid(True)
    plt.title(f'Histogram of fire angle for {name.split("_")[0]}')
    plt.savefig(f'../output/angle/{name}.{output_type}', dpi=resolution)
    plt.clf()
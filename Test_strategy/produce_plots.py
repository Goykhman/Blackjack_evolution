import csv
import matplotlib.pyplot as plt
import numpy as np

with open('player_time_series.csv', 'r') as file:
    reader = csv.reader(file)
    time_series = list(reader)

time_series=time_series[0]

time_series=[int(x) for x in time_series]

time_series_plot=plt.figure(dpi=300)
plt.plot(range(len(time_series)),time_series)
plt.xlabel("rounds")
plt.ylabel("player's bankroll")
plt.title("Bet $2 each round in considered basic strategy")
plt.show()
time_series_plot.savefig("player_bankroll_time_series.pdf",\
                    dpi=300, facecolor='w', edgecolor='w',\
                    orientation='portrait', papertype=None, format=None,\
                    transparent=False, bbox_inches=None, pad_inches=0.1,\
                    frameon=None)

with open('edges.csv', 'r') as file:
    reader = csv.reader(file)
    edges = list(reader)

edges=edges[0]
edges=[float(x) for x in edges]
edges_cropped=[x for x in edges if abs(x)<0.15]
edges_cropped=np.array(edges_cropped)
hist_edges=plt.figure(figsize=(8,5))
plt.hist(edges_cropped,bins=50)
plt.xlabel("edge")
plt.ylabel("count")
mn,std=edges_cropped.mean(),edges_cropped.std()
mn=int(1000*mn)/1000.0
std=int(1000*std)/1000.0
plt.title("Player's edge in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
hist_edges.savefig("edges.pdf",\
                         dpi=300, facecolor='w', edgecolor='w',\
                         orientation='portrait', papertype=None, format=None,\
                         transparent=False, bbox_inches=None, pad_inches=0.1,\
                         frameon=None)

with open('tot_wins.csv', 'r') as file:
    reader = csv.reader(file)
    tot_wins = list(reader)

tot_wins=tot_wins[0]
tot_wins=[float(x) for x in tot_wins]
tot_wins_cropped=tot_wins
tot_wins_cropped=np.array(tot_wins_cropped)
hist_tot_wins=plt.figure(figsize=(8,5))
plt.hist(tot_wins_cropped,bins=50)
plt.xlabel("total win probability")
plt.ylabel("count")
mn,std=tot_wins_cropped.mean(),tot_wins_cropped.std()
mn=int(1000*mn)/1000.0
std=int(1000*std)/1000.0
plt.title("Player's total wins in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
hist_tot_wins.savefig("tot_win.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)

with open('tot_losses.csv', 'r') as file:
    reader = csv.reader(file)
    tot_losses = list(reader)

tot_losses=tot_losses[0]
tot_losses=[float(x) for x in tot_losses]
tot_losses_cropped=tot_losses
tot_losses_cropped=np.array(tot_losses_cropped)
hist_tot_losses=plt.figure(figsize=(8,5))
plt.hist(tot_losses_cropped,bins=50)
plt.xlabel("total loss probability")
plt.ylabel("count")
mn,std=tot_losses_cropped.mean(),tot_losses_cropped.std()
mn=int(1000*mn)/1000.0
std=int(1000*std)/1000.0
plt.title("Player's total losses in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
hist_tot_losses.savefig("tot_losses.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)

with open('player_bankrolls.csv', 'r') as file:
    reader = csv.reader(file)
    player_bankrolls = list(reader)
player_bankrolls=player_bankrolls[0]
player_bankrolls=[int(x) for x in player_bankrolls]
player_bankrolls=np.array(player_bankrolls)
hist_player_bankrolls=plt.figure(figsize=(8,5))
plt.hist(player_bankrolls,bins=50)
plt.xlabel("final bankroll")
plt.ylabel("count")
mn,std=player_bankrolls.mean(),player_bankrolls.std()
mn=int(10*mn)/10.0
std=int(10*std)/10.0
plt.title("Player's bankroll in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
hist_player_bankrolls.savefig("player_final_bankroll.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)



with open('prob_double_down.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)
data=data[0]
data=[float(x) for x in data]
data=np.array(data)
fig=plt.figure(figsize=(8,5))
plt.hist(data,bins=50)
plt.xlabel("probability of double down")
plt.ylabel("count")
mn,std=data.mean(),data.std()
mn=int(100*mn)/100.0
std=int(1000*std)/1000.0
plt.title("Probability of double down in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
fig.savefig("double_down_probability.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)

with open('prob_double_down_won.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)
data=data[0]
data=[float(x) for x in data]
data=np.array(data)
fig=plt.figure(figsize=(8,5))
plt.hist(data,bins=50)
plt.xlabel("probability of winning double down")
plt.ylabel("count")
mn,std=data.mean(),data.std()
mn=int(100*mn)/100.0
std=int(100*std)/100.0
plt.title("Probability of winning double down considered in basic strategy, mean={}, std={}".format(mn,std))
plt.show()
fig.savefig("double_down_win_probability.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)

with open('prob_double_down_loss.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)
data=data[0]
data=[float(x) for x in data]
data=np.array(data)
fig=plt.figure(figsize=(8,5))
plt.hist(data,bins=50)
plt.xlabel("probability of losing double down")
plt.ylabel("count")
mn,std=data.mean(),data.std()
mn=int(100*mn)/100.0
std=int(100*std)/100.0
plt.title("Probability of losing double down in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
fig.savefig("double_down_loss_probability.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)

with open('prob_split.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)
data=data[0]
data=[float(x) for x in data]
data=np.array(data)
fig=plt.figure(figsize=(8,5))
plt.hist(data,bins=50)
plt.xlabel("probability of split")
plt.ylabel("count")
mn,std=data.mean(),data.std()
mn=int(100*mn)/100.0
std=int(1000*std)/1000.0
plt.title("Probability of splitting in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
fig.savefig("split_probability.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)

with open('prob_split_won.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)
data=data[0]
data=[float(x) for x in data]
data=np.array(data)
fig=plt.figure(figsize=(8,5))
plt.hist(data,bins=50)
plt.xlabel("probability of winning a split")
plt.ylabel("count")
mn,std=data.mean(),data.std()
mn=int(100*mn)/100.0
std=int(100*std)/100.0
plt.title("Probability of winning split in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
fig.savefig("split_win_probability.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)

with open('prob_split_loss.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)
data=data[0]
data=[float(x) for x in data]
data=np.array(data)
fig=plt.figure(figsize=(8,5))
plt.hist(data,bins=50)
plt.xlabel("probability of losing a split")
plt.ylabel("count")
mn,std=data.mean(),data.std()
mn=int(100*mn)/100.0
std=int(100*std)/100.0
plt.title("Probability of losing split in considered basic strategy, mean={}, std={}".format(mn,std))
plt.show()
fig.savefig("split_loss_probability.pdf",\
                   dpi=300, facecolor='w', edgecolor='w',\
                   orientation='portrait', papertype=None, format=None,\
                   transparent=False, bbox_inches=None, pad_inches=0.1,\
                   frameon=None)

print ct2,ctmin2,ct4,ctmin4,ct3

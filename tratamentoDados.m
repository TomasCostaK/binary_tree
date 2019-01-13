% This program is capable of reading and analyzing the information given by running binary_tree.c
% 3 Figures: Tree height; number of leaves; calls on
% Main Formula for fit: pinv(log10(n):1+0*n)*val

%% Load file that we made while running binary_tree
InfoCols=load('results.data');

%% Read N nodes
n=InfoCols(:,1);

% Read Tree Height values
TreeHeightMin=InfoCols(:,2);
TreeHeightMax=InfoCols(:,3);
TreeHeightAvg=InfoCols(:,4);

% Read Tree Leaves values
TreeLeavesMin=InfoCols(:,6);
TreeLeavesMax=InfoCols(:,7);
TreeLeavesAvg=InfoCols(:,8);

% Read calls on hit and miss respectively
callsOnHit=InfoCols(:,10);
callsOnMiss=InfoCols(:,12);

clear InfoCols;

%Tree Height Figure
intv = [log10(n),1+0*n];
frm = pinv(intv);
BinTreeHeightMin= frm * TreeHeightMin;
BinTreeHeightMax= frm * TreeHeightMax;
BinTreeHeightAvg= frm * TreeHeightAvg;

figure(1)
plot(n,TreeHeightMin, 'b');
hold on
plot(n,TreeHeightMax, 'r');
hold on
plot(n,TreeHeightAvg, 'g');
hold on
plot(n,intv*BinTreeHeightMin, 'k');
hold on
plot(n,intv*BinTreeHeightMax, 'c');
hold on
plot(n,intv*BinTreeHeightAvg, 'm');
hold off

title('Tree height according to number of elements')
xlabel('Number of elements') 
ylabel('Tree height')

%% Calls on Hit and on Miss

BinTreeHits= frm * callsOnHit;
BinTreeMiss= frm * callsOnMiss;

figure(2)
plot(n,callsOnHit, 'b');
hold on
plot(n,callsOnMiss, 'r');
hold on
plot(n,intv*BinTreeHits, 'g');
hold on
plot(n,intv*BinTreeMiss, 'k');
hold off

title('Number of Hits or Misses according to number of elements')
xlabel('Number of Hit/Miss') 
ylabel('Number of function calls')

%% Number of leaves

intv = [n,1+0*n];
frm = pinv(intv);
BinTreeLeaveMin= frm * TreeLeavesMin;
BinTreeLeavesMax= frm * TreeLeavesMax;
BinTreeLeavesAvg= frm * TreeLeavesAvg;

figure(3)
plot(n,TreeLeavesMin, 'b');
hold on
plot(n,TreeLeavesMax,'r');
hold on
plot(n,TreeLeavesAvg, 'g');
hold on
plot(n,intv*BinTreeLeaveMin, 'k');
hold on
plot(n,intv*BinTreeLeavesMax, 'm');
hold on
plot(n,intv*BinTreeLeavesAvg, 'c');
hold off

title('Number of leaves according to number of elements')
xlabel('Number of elements') 
ylabel('Number of leaves')
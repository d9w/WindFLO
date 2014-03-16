function [fit,sent,received,xcov,xmin]=runCMAES(connections,nnbor)
model=1;
load FarmParameters
load WindResource2
load initpos

%% purecmaes arguments
if nargin < 1 
    connections={}; 
end
if nargin < 2
    nnbor=2;
end
N=400;
stopeval=2000;
numpops=2;
scaling=2.5;
stepsize=2;

%% initial population - fitfunctions
%{
N=50;
xmean=5-10*rand(1,N);
strfitnessfct='fgeneric';
fgeneric('initialize',24,1,'./'); % just for fgeneric function
fctin={};
%}

%% initial population - wind
pop=[];
initpos1=initpos/scaling;
for kl=1:1:length(initpos1(:,1))
    pop=[pop initpos1(kl,:)];
end
xmean=pop;
popv=pop;
strfitnessfct='fitnessWTLayoutnew';
fac=pi/180;
tpositions(:,1)=popv(1,1:2:end)';
tpositions(:,2)=popv(1,2:2:end)';
coses=cos(mean(thetas,2)*fac);
sins=sin(mean(thetas,2)*fac);
largecos=repmat(coses,1,length(tpositions(:,1)));
largesin=repmat(sins,1,length(tpositions(:,1)));
%change the next statement to have transpose 
bcoses=reshape(largecos',prod(size(largecos)),1);
csins=reshape(largesin',prod(size(largesin)),1);
dcos=repmat(bcoses,1,length(tpositions(:,1)));
esin=repmat(csins,1,length(tpositions(:,1)));
%power model
vints=3.5:0.5:vRated;
v1=vints(2:1:end);
v2=vints(1:1:length(vints)-1);
%v=(v1+v2)/2;
v1large=repmat(v1',length(thetas(:,1)),length(tpositions(:,1)));
v2large=repmat(v2',length(thetas(:,1)),length(tpositions(:,1)));
for ghh=2:1:length(vints)
    v=(vints(ghh)+vints(ghh-1))/2;
    P(ghh-1)=PowOutput(v,model,vCin,vCout,vRated,PRated);
end
Plarge=repmat(P',length(thetas(:,1)),length(tpositions(:,1)));

%function [EnergyCapture,particles]=fitnessWTLayoutnew(particles,dcos,esin,Plarge,v1large,v2large)
fctin={dcos,esin,Plarge,v1large,v2large};

%% run purecmaes
[fit,sent,received,xcov,xmin]=purecmaes(N,xmean,strfitnessfct,fctin,...
    nnbor,stopeval,stepsize,connections,numpops,'some');

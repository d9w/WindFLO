function farmsettings = SettingMatrices(tpositions)
% [dcos,esin,Plarge,v1large,v2large]=SettingMatrices(tpositions)
% This should be done at the beginning and not every iteration
load WindResource2
load FarmParameters
model=1;
fac=pi/180;
coses=cos(mean(thetas.*fac,2));
sins=sin(mean(thetas.*fac,2));
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

%% make the farm settings struct
farmsettings.dcos = dcos;
farmsettings.esin = esin;
farmsettings.Plarge = Plarge;
farmsettings.v1large = v1large;
farmsettings.v2large = v2large;

function invE=fitnessWTLayoutnew(particles,dcos,esin,Plarge,v1large,v2large)
%[EnergyCapture,TSpE]=fitnessWTLayoutnew(particles,dcos,esin,Plarge,v1large,v2large)
% This sets up the large matrices that can be reused. If this eval function
% is run inside a loop, it might be beneficial to move this function
% upfront and pass its outputs as inputs to the fitnesslayoutnew function
% as inputs in every iteration. 
%[dcos,esin,Plarge,v1large,v2large]=SettingMatrices(tpositions);
%tpositions(:,1)=particles(1:2:end);
%tpositions(:,2)=particles(2:2:end); 
tpositions=reshape(particles,[],2);
nTurbines=length(tpositions(:,1));    
if nTurbines>2000
    constraint=0;
else
    constraint=CheckConstraint(tpositions)
end

%constraint=1;
if constraint==0      
    TotalEnergy=1e-5; 
    TSpE=zeros(24,2500);
elseif constraint==1
    [done]=WindResourcePerTurbinenew(tpositions, dcos, esin);
    [TotalEnergy,TSpE]=AnnualPowerOutput(nTurbines, Plarge, v1large, v2large); 
end
invE=1./TotalEnergy;

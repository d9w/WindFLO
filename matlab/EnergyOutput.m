function [TSpE EnergyCapture]=EnergyOutput(tpositions,flag)
 
if flag==1
    tpositions=[ tpositions; 1000000 1000000];
    [dcos,esin,Plarge,v1large,v2large]=SettingMatrices(tpositions);
    Re_tpos=reshape(tpositions',1,[]);
    [EnergyCapture,Re_tpos,TSpE]=fitnessWTLayoutnew(Re_tpos,dcos,esin,Plarge,v1large,v2large);
    EnergyCapture=EnergyCapture/2;
    TSpE=TSpE(:,1); 
else 
    %tpositions=[ 0 0]
    [dcos,esin,Plarge,v1large,v2large]=SettingMatrices(tpositions);
    Re_tpos=reshape(tpositions',1,[]);
    [EnergyCapture TSpE]=fitnessWTLayoutnew(Re_tpos,dcos,esin,Plarge,v1large,v2large);
end 

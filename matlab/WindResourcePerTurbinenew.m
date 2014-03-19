% This function will change the 'c' parameter of the weibull
% distribution/direction based on the turbine locations and the wind
% direction. Given the farm level resource data, this will comeup with the
% resource data at each turbine level due to the certain placement of the
% turbines. 
function [done]=WindResourcePerTurbinenew(tpositions,dcos,esin)
load WindResource2
load FarmParameters
%for i=1:1:length(tpositions(:,1))
    %index=i;
    xx=tpositions(:,1)';
    yy=tpositions(:,2)';
    [xx1,xx2]=meshgrid(xx);
    [yy1,yy2]=meshgrid(yy);
    const1=R/k;
    xd=(xx1-xx2)';
    yd=(yy1-yy2)';
    %create differential matrices--Kalyan 
    %changed it to 1, added transpose to this operation 
    xlarge=repmat(xd,[length(thetas(:,1)) 1]);
    ylarge=repmat(yd,[length(thetas(:,1)) 1]);
    
      
    %now create the beta function 
    alpha=atan(k);
    'alpha'
    size(alpha)
    beta=acos((xlarge.*dcos+ylarge.*esin +const1)./sqrt((xlarge+const1*dcos).^2+(ylarge+const1*esin).^2)); 
    
    %Here changing to <0.0000001 because of precision issues in the above 
    ghj=find(beta<0.000001);
    
    %check for the presence of the wake 
    size(beta)
    size(alpha)
    wakepresence=beta<=alpha;
    %gets the indices for the diagnol back to zero<< if my goal is to just
    %make the diagnol as zero since it should not wake itself then I should
    %just be accessing the diagnol elements of the matrix directly, but I
    %think this is not a diagnol, it is the diagnol across several
    %concatenated matrices 
    wakepresence(ghj)=0;
    %calculate the projected distance 
    pjdist=abs(xlarge.*dcos+ylarge.*esin);
    % calculate the wake effects 
    a=1-sqrt(1-CT);
    b=k/R;
    VelDef=a./((1+b.*pjdist).^2);
    %add up the wake based on its presence 
    wk=wakepresence.*VelDef;
    size(wakepresence)
    size(VelDef)
    TotalVdef=sqrt(sum((wk.^2),2));
    % change the wind resource 
    %This was what was causing the error-Kalyan, February 2011
    %crep=repmat(c,1,length(tpositions(:,1)));
    temp1=repmat(c,[length(tpositions(:,1)) 1]);
    crep=reshape(temp1,prod(size(temp1)),1);
    size(crep)
    size(TotalVdef)
    crep=crep.*(1-TotalVdef);
    lp=1;
      vints=3.5:0.5:vRated;
      wholecs=[];
      wholeks=[];
      wholeoms=[];
      wholethets=[];
      smallcs=[];
     smallks=[];
     smalloms=[];
     smallthets=[];
    for i=1:1:length(tpositions(:,1))
        cs(i,:)=crep(lp:length(tpositions(:,1)):end)';
        kk=crep(lp:length(tpositions(:,1)):end)';
        kss(i,:)=ks;
        kkss=ks;
        lp=lp+1;
        thets(i,:)=(thetas(:,2)-thetas(:,1))';
        omegs(i,:)=omegas;       
        
        largec=repmat(kk',1,length(vints)-1);
    largeks=repmat(kkss',1,length(vints)-1);
    %change the next statement to have transpose 
    bc=reshape(largec',prod(size(largec)),1);
    cks=reshape(largeks',prod(size(largeks)),1);
    
    
    largeom=repmat(omegs(i,:)',1,length(vints)-1);
    largethets=repmat(thets(i,:)',1,length(vints)-1);
    %change the next statement to have transpose 
    doms=reshape(largeom',prod(size(largeom)),1);
    ethets=reshape(largethets',prod(size(largethets)),1);
    

     wholecs=[wholecs bc];
     wholeks=[wholeks cks];
     wholeoms=[wholeoms doms];
     wholethets=[wholethets ethets];
     smallcs=[smallcs cs(i,:)'];
     smallks=[smallks kss(i,:)'];
     smalloms=[smalloms omegas'];
     smallthets=[smallthets thets(i,:)'];
     
        
        
     end
    

save TurbineSpecResourceAlt wholecs wholeks wholeoms  wholethets
save TurbineSpecResource smallcs smallks smalloms smallthets
    done =1;
    

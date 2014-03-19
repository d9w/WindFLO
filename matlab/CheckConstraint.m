function [constraint,tpositions, MD, D]=CheckConstraint(tpositions)
%this can be changed into a MOO problem very easily
% s1=(tpositions(:,1).^2+tpositions(:,2).^2)-(190000^2);
% indices=find(s1<=0);
% s2=length(indices);
load FarmParameters
constraint=1;
c=0;

[D]=Mdist(tpositions);
%fixing the diagnoals since they will have zero distance as it their own
%distance from itself. 
tempvec=65*R^2*ones(1,length(tpositions(:,1)));
RMat=diag(tempvec);
D=D+RMat;
%Diagnols are fixed. 
MD=zeros(size(D));
check=find(D<=8*R);
if isempty(check)
    constraint=1;
else constraint=0;
end
MD(check)=1;
%MD=MD-RMat;
% if constraint==0
%     cases=3;
%  [tpositions]=FixInfeasible(cases,tpositions,MD,D);
% end
% for hj=1:1:length(tpositions(:,1))
%     x1=tpositions(hj,1);
%     y1=tpositions(hj,2);
%     ind=1:1:length(tpositions(:,1));
%     others=setxor(ind,hj);
%     for hjh=1:1:length(others)
%         dist=(x1-tpositions(others(hjh),1))^2+(y1-tpositions(others(hjh),2))^2;
%         if dist<64*R^2
%             c=1;
%         end 
%     end 
%     
% %     if s2<length(tpositions(:,1))
% %         constraint=0;
% %     end
%     if c==1
%         constraint=0;
%     end
%     
%     
% end

function init = initGrid(xmax,ymax,n)
% makes a grid x by y large with n turbines evenly spaced

%% determine row and column
y=floor(sqrt(n*ymax/xmax));
x=ceil(n/y);
extra=x*y-n;

%% make sure there's an extra column
while extra<0
    x=x+1;
    extra=x*y-n;
end

%% make grid
xs=xmax/x:xmax/x:xmax;
ys=ymax/y:ymax/y:ymax;

init(:,2)=repmat(ys(:),x,1);
for i=1:x
    init(((i-1)*y+1):(i*y+1),1)=repmat(xs(i),y+1,1);
end

%% take only n turbines
init=init(1:n,:);

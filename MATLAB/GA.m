function GA(wfle)

% set up grid so that the centers are > 8*wfle.ws.R apart
interval = 8.001 * wfle.ws.R;
xs = 0:interval:wfle.ws.width;
ys = 0:interval:wfle.ws.height;

grid = [reshape(repmat(xs,length(ys),1)',[],1) ...
    reshape(repmat(ys,length(xs),1),[],1)];

valid=[];
for i=1:size(grid,1)
    v = 1;
    for o=1:size(wfle.ws.obstacles,1)
        if all([grid(i,:)>wfle.ws.obstacles(o,1:2) ...
                grid(i,:)<wfle.ws.obstacles(o,3:4)])
            v=0;
        end
    end
    if v
        valid = [valid; grid(i,:)];
    end
end

grid = valid;
max_turbs = size(grid,1);

num_pop = 20;       % populations
tour_size = 4;      % tournament size
mut_rate = 0.05;    % mutation rate
cross_rate = 0.40;  % crossover rate - uniform crossover

pops=zeros(max_turbs, num_pop);
fits=zeros(num_pop,1);

% initialize the populations
for p=1:num_pop
    bins = zeros(size(grid,1),1);
    bins(randperm(max_turbs,randi(max_turbs))) = 1;
    pops(:,p) = bins;
    layout = grid(logical(bins),:);
    wfle = wfle.evaluate(layout);
    fits(p) = length(wfle.TurbineFitnesses(wfle.TurbineFitnesses>0.8));
end

% run the GA
for i=2:(1000/num_pop)
    
    % rank
    winners = zeros(num_pop/tour_size,1);
    indices = 1:num_pop;
    for t=1:length(winners)
        indices=indices(randperm(length(indices)));
        tourney=indices(1:tour_size);
        winners(t)=find(fits==max(fits(tourney)),1);
        indices(1:tour_size)=[];
    end 
    
    children = zeros(max_turbs, num_pop);
    % crossover
    for c=length(winners):num_pop
        parents=winners(randperm(length(winners),2));
        child=pops(:,parents(1));
        crossed=rand(max_turbs,1)<cross_rate;
        child(crossed)=pops(crossed,parents(2));
        children(:,c)=child;
    end
    
    % mutate
    opposite=1-children;
    mutate=rand(size(children))<mut_rate;
    children(mutate)=opposite(mutate);
    
    % elitism
    for c=1:length(winners)
        children(:,c)=pops(:,winners(c));
    end
    pops = children;
    fits = zeros(num_pop,1);
    
    % evaluate
    for p=1:num_pop
        bins = pops(:,p);
        layout = grid(logical(bins),:);
        wfle = wfle.evaluate(layout);
        fits(p)=length(wfle.TurbineFitnesses(wfle.TurbineFitnesses>0.8));
    end
    
    disp([i max(fits)]);
end

end
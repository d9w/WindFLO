function cost=windCost(energy,turbines);
% simple cost function meant to combine turbine energy output and cost in a
% nonlinear fashion

tcost=750000;    % $750,000/turbine
scost=8000000;   % $8,000,000/substation
M=30;            % turbines/substation
rev=0.08;        % revenuw per kwh
om=20000;        % operation and maintenance/turbine

cost=energy*8760*20*1000*rev-(turbines*tcost+floor(turbines/M)*scost)-om*turbines*20;
if isnan(cost)
    cost=0;
end
disp([energy turbines cost])

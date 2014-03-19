% Cross validate
% Check that layouts return the wfratio expected

fid=fopen('../TestLayouts/results.txt');
results=fscanf(fid,'%f');
fclose(fid);

for i=0:99
    fid=fopen(['../TestLayouts/layout' num2str(i) '.txt']);
    A=fscanf(fid,'%f');
    fclose(fid);
    layout=[A(2:2:end) A(3:2:end)];
    for sc=0:8
        wfle = WindFarmLayoutEvaluator(sc);
        wfle = wfle.evaluate(layout);
        if abs(results(i*9+sc+1)-wfle.wfRatio) > 0.01
            disp(['error: ' num2str(i) ' ' num2str(sc)])
            disp([results(i*sc+sc+1) wfle.wfRatio])
        end
    end
end
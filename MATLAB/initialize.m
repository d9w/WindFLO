function ws=initialize(WindScenario)
% returns a struct with the farm parameters based
%   set parameters such as farm size
%   wind parameters - the weibull distribution parameters

%% these parameters are farm-wide and not dependent on wind
ws.CT=0.8;
ws.farmRadius=500.0;
ws.PRated=1500.0;
ws.R=38.5;
ws.eta=-500.0;
ws.k=0.0750;
ws.lambda=140.86;
ws.vCin=3.5;
ws.vCout=20;
ws.vRated=14;

%% load the wind resource file

try
    docstruct = xml2struct(WindScenario);
catch
    error(['Error reading scenario ' WindScenario])
end

cs = [];
ks = [];
omegas = [];
thetas = [];
ws.obstacles = [];

for i=1:length(docstruct.Children)
    class = docstruct.Children(i);
    if strcmp(class.Name,'Angles')
        for a=1:length(class.Children)
            angle = class.Children(a);
            if strcmp(angle.Name, 'angle')
                for attr=1:length(angle.Attributes)
                    eval([angle.Attributes(attr).Name 's=[' ...
                        angle.Attributes(attr).Name 's ' ...
                        angle.Attributes(attr).Value '];']);
                end
            end
        end
    end
    if strcmp(class.Name, 'Obstacles')
        for o=1:length(class.Children)
            obstacle = class.Children(o);
            if strcmp(obstacle.Name, 'obstacle')
                xmin=-1;ymin=-1;xmax=-1;ymax=-1;
                for attr=1:length(obstacle.Attributes)
                    eval([obstacle.Attributes(attr).Name '=' ...
                        obstacle.Attributes(attr).Value ';']);
                end
                obdims = [xmin ymin xmax ymax];
                if all(obdims >= zeros(1,4))
                    ws.obstacles = [ws.obstacles; obdims];
                end
            end
        end
    end
    if strcmp(class.Name, 'Parameters')
        for p=1:length(class.Children)
            parameter = class.Children(p);
            if strcmp(parameter.Name, 'Width')
                ws.width = str2double(parameter.Children.Data);
            end
            if strcmp(parameter.Name, 'Height')
                ws.height = str2double(parameter.Children.Data);
            end
            if strcmp(parameter.Name, 'NTurbines')
                ws.nturbines = str2double(parameter.Children.Data);
            end
            if strcmp(parameter.Name, 'WakeFreeEnergy')
                ws.energy = str2double(parameter.Children.Data);
            end
        end
    end
end

ws.cs = cs;
ws.ks = ks;
ws.omegas = omegas;
ws.thetas = [thetas' thetas'+15];
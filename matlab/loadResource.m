function [cs, ks, omegas, thetas, energy] = loadResource(sc_num)

try
    docstruct = xml2struct(['../Scenarios/' sc_num '.xml']);
catch
    error(['Error reading scenario ' sc_num '.xml'])
end

cs = [];
ks = [];
omegas = [];
thetas = [];
energy = 0;

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
        % Not used currently
    end
    if strcmp(class.Name, 'Parameters')
        for p=1:length(class.Children)
            parameter = class.Children(p);
            if strcmp(parameter.Name, 'WakeFreeEnergy')
                energy = parameter.Children.Data;
            end
        end
    end
end
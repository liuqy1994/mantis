% This Source Code Form is subject to the terms of the Mozilla Public
% License, v. 2.0. If a copy of the MPL was not distributed with this
% file, You can obtain one at https://mozilla.org/MPL/2.0/.

% This Source Code Form is subject to the terms of the Mozilla Public
% License, v. 2.0. If a copy of the MPL was not distributed with this
% file, You can obtain one at https://mozilla.org/MPL/2.0/.

function [ t, s ] = gen_spline_samples( t_vias, vias, order, num_samples )
%GEN_DATA_POINT_STRUCT Summary of this function goes here
%   Detailed explanation goes here
    
    % Pre-allocate vectors
    t = zeros(1,0);
    s = zeros(5,0);
    
    for i = 2:size(vias,2)
        
        % Segment times
        st0 = t_vias(i-1);
        stf = t_vias(i);
        sdt = stf-st0;
        
        st = linspace(0, 1, num_samples);  
        
        switch(order)
            case 3
               as = spline_solver_cubic(sdt, ...
                                            vias(1,i-1), ...
                                            vias(2,i-1), ...
                                            vias(1,i), ...
                                            vias(2,i));
            case 5
                as = spline_solver_quintic(sdt, ...
                                            vias(1,i-1), ...
                                            vias(2,i-1), ...
                                            vias(3,i-1), ...
                                            vias(1,i), ...
                                            vias(2,i), ...
                                            vias(3,i));
            case 7
                as = spline_solver_septic(sdt, ...
                                            vias(1,i-1), ...
                                            vias(2,i-1), ...
                                            vias(3,i-1), ...
                                            vias(4,i-1), ...
                                            vias(1,i), ...
                                            vias(2,i), ...
                                            vias(3,i), ...
                                            vias(4,i));
            case 9
                as = spline_solver_nonic(sdt, ...
                                            vias(1,i-1), ...
                                            vias(2,i-1), ...
                                            vias(3,i-1), ...
                                            vias(4,i-1), ...
                                            vias(5,i-1), ...
                                            vias(1,i), ...
                                            vias(2,i), ...
                                            vias(3,i), ...
                                            vias(4,i), ...
                                            vias(5,i));
            otherwise
                error('Unknown spline order')
        end


        %% Spline Calculator

        nt = linspace(0, 1, num_samples);  
        a = zeros(10,1);
        a(1:length(as)) = as;
        a = flip(a)';   % We want decending order (spline_calculator) and horizontal (for polyder)
        sq = spline_calculator(nt, sdt, a);
                
        st = linspace(st0, stf, num_samples);
        t = [t(1:end-1), st];
        s = [s(:,1:end-1), sq];
    end
end

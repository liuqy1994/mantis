% This Source Code Form is subject to the terms of the Mozilla Public
% License, v. 2.0. If a copy of the MPL was not distributed with this
% file, You can obtain one at https://mozilla.org/MPL/2.0/.

function [  ] = function_gen_mat( m, m_name )
%FUNCTION_GEN_MAT Generates a C++ function to calculate matrices
%   Detailed explanation goes here

    t = char(9);
    n = char(10);
    
    var_str = [];
    
    try
        var_str = symvar(m); %Try to get out the variables in the matrix
    catch ex
        if ~strcmp(ex.identifier, 'MATLAB:catenate:dimensionMismatch')
            error(ex) %If it's not the error we expect, error
        else    
            m = sym(m); %else, the function was probably passed a static matrix
        end
    end

    %Generate function call
    fun_str = ['#include <eigen3/Eigen/Dense>', n, n, 'inline void calc_', m_name, '(Eigen::MatrixXd& ', m_name];
    for i = 1:numel(var_str)
        fun_str = [fun_str, ', double ', char(var_str(i))];
    end
    fun_str = [fun_str, ') {', n];

    %Generate function code
    for i = 1:numel(m)
        [j,k] = ind2sub(size(m),i);
        j = num2str(j-1);
        k = num2str(k-1);

        code = strsplit(ccode(m(i)),'=');
        code_str = code{2};

        %Don't bother adding 'zero' strings
        if ~strcmp(code_str,' 0.0;')
            fun_str = [fun_str, t, m_name, '(', j, ',', k, ') =', code_str, n];
        end
    end

    %Close function call
    fun_str = [fun_str, '}', n];

    %Save function to disk
    filename = ['./gen_code/calc_', m_name, '.h'];
    fid = fopen(filename, 'w');

    if fid ~= -1
        fprintf(fid, fun_str);
        fclose(fid);
    else
        error(['Could not create file: ', filename]);
    end
end


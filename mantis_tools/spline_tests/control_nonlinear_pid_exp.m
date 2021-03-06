% This Source Code Form is subject to the terms of the Mozilla Public
% License, v. 2.0. If a copy of the MPL was not distributed with this
% file, You can obtain one at https://mozilla.org/MPL/2.0/.

% This Source Code Form is subject to the terms of the Mozilla Public
% License, v. 2.0. If a copy of the MPL was not distributed with this
% file, You can obtain one at https://mozilla.org/MPL/2.0/.

function [ tau, acc_c, q_c, ang_integrator, omega_integrator ] = control_nonlinear_pid_exp(pos_sp, vel_sp, acc_sp, yaw_sp, x, x_p, dt, ang_integrator, omega_integrator)
%CONTROL_NONLINEAR_PID_EXP Summary of this function goes here
%   Detailed explanation goes here

    % Hardcode gains here, as they do in PX4
    KxP = 1;
    KxdP = 2;
    MC_ANG_P = 6.5;
    MC_RATE_P = 7.5;
    MC_RATE_I = 2.5;
    MC_RATE_D = 0.15;
    yaw_w = 0.6;

    % Gain Scaling
    % This why we pick gains for MC_RATE_P, etc., to be 50-100x larger than
    % the ones used in the PX4 firmware.
    gs = 1/50;
            
    % Instead of normalising the thrust vector, simply use F=ma
    kT = calc_total_mass(model);
    
    sn = state_names_lookup(model.NB-6);
    R = quat2rotm(x(sn.STATE_Q)');

    %% High-Level 
    [ vd_b, acc_c, ~, q_c ] = control_acceleration_vector(pos_sp, vel_sp, acc_sp, yaw_sp, ...       % References
                                                          x(sn.STATE_XYZ), x(sn.STATE_VXYZ), R, ... % States
                                                          KxP, KxdP);                               % Gains

    %% Low-Level
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %     % PX4 PID Tracking
    %     MC_ANG_P = 6.5;
    %     MC_RATE_P = 7.5;
    %     MC_RATE_I = 2.5;
    %     MC_RATE_D = 0.15;
    %     KP_t = MC_ANG_P*MC_RATE_P;
    %     KI_t = MC_RATE_I*MC_ANG_P;
    %     KD_t = MC_RATE_P;
    %     KI = MC_RATE_I;
    %     KD = MC_RATE_D;
    %
    %     c(CONTROL_R_E,i) = error_q_att(q_sp_c, x(STATE_Q,i-1)', yaw_w);
    %     c(CONTROL_WXYZ_B,i) = zeros(3,1);
    %     ew = c(CONTROL_WXYZ_B,i) - x(STATE_WXYZ_B,i-1);
    %     ewd = zeros(3,1);
    %     wd_sp = zeros(3,1);
    %     if i > 2
    %         ewd = wd_sp - (x(STATE_WXYZ_B,i-1) - x(STATE_WXYZ_B,i-2))/dt;
    %     end
    %     px4_eang_int = px4_eang_int + c(CONTROL_R_E,i)*dt;
    %     px4_rang_int = px4_rang_int + ew*dt;
    %
    %     % Control Structure resmbles PtIt-PwIw-Dwd
    %     wd_b = KP_t*c(CONTROL_R_E,i) + KD_t*ew + KI_t*px4_eang_int + KI*px4_rang_int + KD*ewd;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    % Convert to normal PID gains
    KP_t = MC_ANG_P*MC_RATE_P;
    KI_t = MC_ANG_P*MC_RATE_I;
    KD_t = MC_RATE_P;
    KI = MC_RATE_I;
    KD = MC_RATE_D;

    w_sp = zeros(3,1);
    wd_sp = zeros(3,1);
    
    eR = error_q_att(q_c, x(sn.STATE_Q)', yaw_w);
    ew = w_sp - x(sn.STATE_WXYZ_B);
    ewd = wd_sp - (x(sn.STATE_WXYZ_B) - x_p(sn.STATE_WXYZ_B))/dt;
    
    ang_integrator = ang_integrator + eR*dt;
    omega_integrator = omega_integrator + ew*dt;

    % Control Structure resmbles PtIt-PwIw-Dwd
    wd_b = KP_t*eR + KD_t*ew + KI_t*ang_integrator + KI*omega_integrator + KD*ewd;
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
%     w_sp = MC_ANG_P*error_q_att(q_c, x(sn.STATE_Q)', yaw_w);
%     wd_sp = zeros(3,1);
% 
%     ew = w_sp - x(sn.STATE_WXYZ_B);
%     ewd = wd_sp - (x(sn.STATE_WXYZ_B) - x_p(sn.STATE_WXYZ_B))/dt;
% 
%     integrator = omega_integrator + ew*dt;
% 
%     wd_b = MC_RATE_P*ew + MC_RATE_I*integrator + MC_RATE_D*ewd;
    
    
    
    %% Gain Reduction Factor
    % Don't use feedback linearisation, instead we simply use a term to
    % scale the gains such that they can be tuned in a nicer fashion.
    %     xid_b = [wd_b;
    %              vd_b];
    
    tau = [gs*wd_b; ...
           kT*vd_b];

end


% This Source Code Form is subject to the terms of the Mozilla Public
% License, v. 2.0. If a copy of the MPL was not distributed with this
% file, You can obtain one at https://mozilla.org/MPL/2.0/.

% This Source Code Form is subject to the terms of the Mozilla Public
% License, v. 2.0. If a copy of the MPL was not distributed with this
% file, You can obtain one at https://mozilla.org/MPL/2.0/.

function [ ew, ewd ] = error_w_wd_trans(  R_sp, R, w_sp, w, wd_sp, wd )
%ERROR_W_TRANS Summary of this function goes here
%   Detailed explanation goes here

%     % Rp -> Rsp
%     eR = R'*R_sp;
%     % ew is the omega error in the body frame at the current time
%     ew = eR*w_sp - w;


%     % Rp -> Rsp
    eR = R'*R_sp;
    eRd = (-vee_up(w)*R')*R_sp + R'*(R_sp*vee_up(w_sp));

%     % ew is the omega error in the body frame at the current time
    ew = eR*w_sp - w;
    ewd = eRd*w_sp + eR*wd_sp - wd;
end



clc

deg2rad = pi/180;
rad2deg = 180/pi;

Kp = 130;
Ki = 0;
Kd = 220;

m = 80;
r = 1.2;
I_g = m*r^2*eye(3);

M_RB = [m*eye(3,3) zeros(3,3);
    zeros(3,3) I_g];
H = [zeros(3,12);
    zeros(3,3) eye(3,3) zeros(3,6);
    zeros(3,12);
    zeros(3,9) eye(3,3)];

h = 0.01;
N = 3000;
t = 0:h:N*h+h;

Q_angle = 0.001;
Q_velocity = 0.003;
R_measure = 0.03;

Q = [zeros(3,12);
    zeros(3,3) Q_angle*eye(3,3) zeros(3,6);
    zeros(3,12);
    zeros(3,9) Q_velocity*eye(3,3)];

R = [zeros(3,12);
    zeros(3,3) R_measure*eye(3,3) zeros(3,6);
    zeros(3,12);
    zeros(3,9) R_measure*eye(3,3)];

x = zeros(12, N+1);
x_est = x;
x_prop = x;

% zero means that we know the exact value at the start, we are not 100%
% sure on the angle measurement
P_prop = [zeros(3,12);
    zeros(3,3) 0.1*eye(3,3) zeros(3,6);
    zeros(6,12)];

for i=1:N+1
    phi = x(1,i);
    theta = x(2,i);
    psi = x(3,i);
    
    [J, J1, J2] = eulerang(phi, theta, psi);
    
    C_RB = [m*Smtrx(x(10:12,i)) zeros(3,3);
        zeros(3,3) -Smtrx(I_g*x(10:12,i))];
    
    size(M_RB\(-C_RB))
    
    f_x = [J zeros(6,6);
        zeros(6,6) M_RB\(-C_RB)]*x(:,i);
    
    B = [zeros(12,12)];
        
    K = P_prop*H'\(H*P_prop*H'+R);
   
    x_est(:,i+1) = x_prop(:,i) + K*[;
    
end

figure(1)
subplot(2,1,1)
plot(t,x(1,:).*rad2deg, t, x(2,:).*rad2deg, t, x(3,:).*rad2deg)
legend('\phi','\theta','\psi')
subplot(2,1,2)
plot(t,x(4,:).*rad2deg, t, x(5,:).*rad2deg, t, x(6,:).*rad2deg)
legend('p','q','r')


fprintf('Next we will see that the controller does not work for any desired values.\n')
fprintf('We now set theta reference to 100 degrees and see what happens in Figure 2\n')
fprintf('This demonstrates the limitation of using Euler angles\n\n')

h = 0.01;
N = 700;
t = 0:h:N*h+h;
x = zeros(6, N+1);
x_ref = [ones(1, N+1)*50*deg2rad; 
        ones(1, N+1)*100*deg2rad; 
        ones(1, N+1)*30*deg2rad];

for i=1:N+1
    phi = x(1,i);
    theta = x(2,i);
    psi = x(3,i);
    
    [J, J1, J2] = eulerang(phi, theta, psi);
    
    x(1:3,i+1) = x(1:3,i) + h*J2*x(4:6,i);
    
    integral = h*[trapz(x_ref(1,1:i)'-x(1,1:i)');trapz(x_ref(2,1:i)'-x(2,1:i)');trapz(x_ref(3,1:i)'-x(3,1:i)')];
    pid = Kp*(x_ref(1:3,i)-(x(1:3,i)))-Kd*x(4:6,i)+Ki*integral;
    
    tau = zeros(3,1)+pid;

    x(4:6,i+1) = x(4:6,i) + h*linsolve(I_g,tau-(-Smtrx(I_g*x(4:6,i))*x(4:6,i))); 
end



figure(2)
subplot(2,1,1)
plot(t,x(1,:).*rad2deg, t, x(2,:).*rad2deg, t, x(3,:).*rad2deg)
legend('\phi','\theta','\psi')
subplot(2,1,2)
plot(t,x(4,:).*rad2deg, t, x(5,:).*rad2deg, t, x(6,:).*rad2deg)
legend('p','q','r')



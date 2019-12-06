function [pos1, pos2, pos3, pos4] = generate(qtd)
pos1 = [];
pos2 = [];
pos3 = [];
pos4 = [];
imax = 90;
for i = 1:qtd
    pos1(i) = randi(imax);
    pos2(i) = randi(imax);
    pos3(i) = randi(imax);
    pos4(i) = randi(imax);
end
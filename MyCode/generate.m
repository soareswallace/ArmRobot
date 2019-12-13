function [pos1, pos2, pos3, pos4] = generate(qtd)
pos1 = [];
pos2 = [];
pos3 = [];
pos4 = [];
imax = 90;
for i = 1:qtd
    pos1(i) = randi([-90 imax]);
    pos2(i) = randi(imax);
    if (pos2(i) < 0)
            pos2(i)=0;
    end
    if (pos2(i) > 85)
            pos2(i) = 85;
    end
    pos3(i) = randi(imax);
    if (pos2(i) + pos3(i) > 95)
            pos3(i) = 95-pos2(i);
    end
    pos4(i) = randi(imax);
    if (pos2(i) + pos3(i) + pos4(i) > 160)
            pos4(i) = 160-(pos2(i) + pos3(i));
    end
end
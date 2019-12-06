for i = 1:50
    disp(strcat('setMultiplePosition(',num2str(round(rand()*90)),', ',num2str(round(rand()*90)),', ',num2str(round(rand()*90)),');'))
    disp(strcat('Serial.println(',num2str(i),');'))
end
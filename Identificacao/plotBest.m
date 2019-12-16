clear all;
bestPerform = Inf;
epochsReached = Inf;
bestRecord = struct([]);
bestNumber = 0;
for count = 1:144
    tr = struct([]);
    file_name = strcat('historico da rede',num2str(count),'.mat')
    var_name = 'tr';
    load(file_name,var_name);
    if ((bestPerform > eval(strcat(var_name,'.best_tperf')))||((bestPerform == eval(strcat(var_name,'.best_tperf')))&&(epochsReached == eval(strcat(var_name,'.best_epoch')))))
       bestPerform = eval(strcat(var_name,'.best_tperf'));
       epochsReached = eval(strcat(var_name,'.best_epoch'));
       bestRecord = eval(strcat(var_name));
       bestNumber = count;
    end
end
plotperform(bestRecord);
bestNumber
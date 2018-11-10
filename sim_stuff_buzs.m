wake_epoch=sessInfo.Epochs.Wake .*1000; %start end wake epoch in ms
spk_ts=sessInfo.Spikes.SpikeTimes .*1000;
spk_id=sessInfo.Spikes.SpikeIDs;

bins=[];
% for i=1:size(wake_epoch)
for i=1
    bins=[bins wake_epoch(i,1):20:wake_epoch(i,2)]; %50 ms bins
end

idx=unique(spk_id);
wake_q=zeros(length(bins)-1,length(idx));

for i=1:length(idx)
    wake_q(:,i)=histcounts(spk_ts(spk_id==idx(i)),bins);
end




pre_epoch=sessInfo.Epochs.NREM(sessInfo.Epochs.NREM(:,1) >= sessInfo.Epochs.PREEpoch(1) & sessInfo.Epochs.NREM(:,1) <= sessInfo.Epochs.PREEpoch(2),:) .*1000; %start end wake epoch in ms

bins=[];
% for i=1:size(pre_epoch)
for i=1
    bins=[bins pre_epoch(i,1):20:pre_epoch(i,2)]; %50 ms bins
end

pre_q=zeros(length(bins)-1,length(idx));

for i=1:length(idx)
    pre_q(:,i)=histcounts(spk_ts(spk_id==idx(i)),bins);
end




post_epoch=sessInfo.Epochs.NREM(sessInfo.Epochs.NREM(:,1) >= sessInfo.Epochs.POSTEpoch(1) & sessInfo.Epochs.NREM(:,1) <= sessInfo.Epochs.POSTEpoch(2),:) .*1000; %start end wake epoch in ms

bins=[];
% for i=1:size(post_epoch)
for i=1
    bins=[bins post_epoch(i,1):20:post_epoch(i,2)]; %50 ms bins
end

post_q=zeros(length(bins)-1,length(idx));

for i=1:length(idx)
    post_q(:,i)=histcounts(spk_ts(spk_id==idx(i)),bins);
end


%%
% r=corr(pre_q);
[assemblies,Z,D,thres]=cluster_mi(pre_q,'prune',4,'plotFlag',false,'shuffle',100,'sig',1,'precision',5,'method','pearson','nofilt',false);
% r(setxor(cell2mat(assemblies),1:length(idx)),setxor(cell2mat(assemblies),1:length(idx)))=0;
r=zeros(length(idx));
for i=1:length(assemblies)
    r(assemblies{i},assemblies{i})=.1*i -1;
end
schemaball(r,[],[1,0,1;1 1 0]);

r=corr(wake_q);
[assemblies,Z,D,thres]=cluster_mi(wake_q,'prune',4,'plotFlag',false,'shuffle',100,'sig',1,'precision',5,'method','pearson','nofilt',false);
r=zeros(length(idx));
for i=1:length(assemblies)
    r(assemblies{i},assemblies{i})=.1*i -1;
end
schemaball(r,[],[1,0,1;1 1 0]);

r=corr(post_q);
[assemblies,Z,D,thres]=cluster_mi(post_q,'prune',4,'plotFlag',false,'shuffle',100,'sig',1,'precision',5,'method','pearson','nofilt',false);
r=zeros(length(idx));
for i=1:length(assemblies)
    r(assemblies{i},assemblies{i})=.1*i -1;
end
schemaball(r,[],[1,0,1;1 1 0]);
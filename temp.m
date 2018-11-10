dec = ca_filt(deconv);

dec(dec==0)=nan;
dec=(dec-mean(dec,'omitnan'))./std(dec,'omitnan') + median(dec(:),'omitnan');

dec(isnan(dec))=0;
import matplotlib.pyplot as plt
from scipy import stats

class NNplotter:

    def PlotLoss(history, output_path = "./loss.png", ret = False):
     
        fig = plt.figure(figsize=(13,7), dpi = 160)

        plt.plot(history.history['loss'], c = 'fuchsia', label = 'Loss Train')
        plt.plot(history.history['val_loss'], c = 'blue', label = 'Loss Validation')
        plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
        plt.gcf().text(0.3, 0.9, r"$Train,Validation,test=\{0.6, 0.1, 0.2\}$", fontsize=15)
        plt.gcf().text(0.78, 0.9, r"$Run: 324997$", fontsize=15)
        plt.xlabel("Epoch", size=15)
        plt.ylabel("Loss", size=15)
        plt.tick_params(axis='both', which='major', labelsize=13)
        leg = plt.legend(loc="upper right", prop={'size': 18})
        leg.get_frame().set_linewidth(0.0)
        
        if ret:
            return fig
        else:
            plt.savefig(output_path)
            
            
            
    def PlotTrainTest(model, x_train, x_test, y_train, y_test, output_path="./DistOutput.png", ret=False):
            
        #distribution for train test and signal/bkg
        y_pred = model.predict(x_test)
        y_pred_train = model.predict(x_train)

        y_pred_sig = []
        y_pred_bkg = []

        for i,j in zip(y_pred, y_test):
            if(j == 1):
                y_pred_sig.append(i[0])
            else:
                y_pred_bkg.append(i[0])
                
        y_pred_sig_t = []
        y_pred_bkg_t = []

        for i,j in zip(y_pred_train, y_train):
            if(j == 1):
                y_pred_sig_t.append(i[0])
            else:
                y_pred_bkg_t.append(i[0])
                

        bkgKStest = stats.ks_2samp(y_pred_bkg, y_pred_bkg_t)
        sigKStest = stats.ks_2samp(y_pred_sig, y_pred_sig_t)

        fig = plt.figure(figsize=(13,7), dpi=160)

        plt.hist( y_pred_sig_t, density=True, range=[0, 1], bins=20, alpha=0.7, color = 'grey', label="Train Signal")
        plt.hist( y_pred_bkg_t, density=True, range=[0, 1], bins=20, alpha=0.3, color = 'grey', label="Train Background")
        n,bins,patches = plt.hist( y_pred_sig, range=[0, 1], histtype='step', density=True, bins=20, color = 'blue', label = "KS: {:.3}, p value: {:.3}".format(sigKStest.statistic, sigKStest.pvalue))
        n,bins,patches = plt.hist( y_pred_bkg, range=[0, 1], histtype='step', density=True, bins=20, color = 'fuchsia', label = "KS: {:.3}, p value: {:.3}".format(bkgKStest.statistic, bkgKStest.pvalue))
        leg = plt.legend(loc="upper center", prop={'size': 15})
        leg.get_frame().set_linewidth(0.0)
        plt.xlabel("DNN output", size=15)
        plt.ylabel("Events/0.02  Normalized to unit area", size = 15)
        plt.tick_params(axis='both', which='major', labelsize=11)
        plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
        plt.gcf().text(0.78, 0.9, r"$Run: 324997$", fontsize=15)
        if ret:
            return fig
        else:
            fig.savefig(output_path)
            
        
    def PlotTest(model, x_test, y_test, output_path="./DistOutputTest.png", ret=False):
        
        #distribution for train test and signal/bkg
        y_pred = model.predict(x_test)

        y_pred_sig = []
        y_pred_bkg = []

        for i,j in zip(y_pred, y_test):
            if(j == 1):
                y_pred_sig.append(i[0])
            else:
                y_pred_bkg.append(i[0])
                

        fig = plt.figure(figsize=(13,7), dpi=160)

        n,bins,patches = plt.hist( y_pred_sig, range=[0, 1], histtype='step', density=True, bins=20, color = 'blue', label = "Signal")
        n,bins,patches = plt.hist( y_pred_bkg, range=[0, 1], histtype='step', density=True, bins=20, color = 'fuchsia', label = "Background")
        leg = plt.legend(loc="upper center", prop={'size': 15})
        leg.get_frame().set_linewidth(0.0)
        plt.xlabel("DNN output", size=15)
        plt.ylabel("Events/0.02  Normalized to unit area", size = 15)
        plt.tick_params(axis='both', which='major', labelsize=11)
        plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
        plt.gcf().text(0.78, 0.9, r"$Run: 324997$", fontsize=15)
        if ret:
            return fig
        else:
            fig.savefig(output_path)


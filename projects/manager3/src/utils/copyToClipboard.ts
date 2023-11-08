/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { Notify } from 'quasar';
import { i18n } from 'boot/i18n';

export const copyToClipboard = (content:string):void => {
  navigator.clipboard.writeText(content)
    .then(():void => {
      Notify.create({
        message: i18n.global.t('general.copyToClipboard'),
        color: 'positive',
        icon: 'mdi-clipboard-check',
      });
    }, (error):void => {
      Notify.create({
        message: error,
        type: 'negative',
        icon: 'mdi-alert-outline',
      });
    });
};

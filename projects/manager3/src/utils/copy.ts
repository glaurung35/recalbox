/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { copyToClipboard, Notify } from 'quasar';
import { i18n } from 'boot/i18n';

export const copy = (content: string) => {
  copyToClipboard(content)
    .then(() => {
      Notify.create({
        message: i18n.global.t('general.copyToClipboard'),
        color: 'positive',
        icon: 'mdi-clipboard-check',
      });
    }, (error) => {
      Notify.create({
        message: error,
        type: 'negative',
        icon: 'mdi-alert-outline',
      });
    });
};

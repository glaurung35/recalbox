import { Notify } from 'quasar';
import { i18n } from 'boot/i18n';

export const copyToClipboard = (content) => {
  navigator.clipboard.writeText(content)
    .then(() => {
      Notify.create({
        message: i18n.global.t('general.copyToClipboard'),
        color: 'primary',
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

export const compare = (a, b) => {
  // Use toUpperCase() to ignore character casing
  const labelA = a.label.toUpperCase();
  const labelB = b.label.toUpperCase();

  let comparison = 0;
  if (labelA > labelB) {
    comparison = 1;
  } else if (labelA < labelB) {
    comparison = -1;
  }
  return comparison;
};

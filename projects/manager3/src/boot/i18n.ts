/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { boot } from 'quasar/wrappers';
import { createI18n } from 'vue-i18n';
import { LocalStorage, Quasar } from 'quasar';
import messages from 'src/i18n';

export type MessageLanguages = keyof typeof messages;
// Type-define 'fr-FR' as the master schema for the resource
export type MessageSchema = typeof messages['fr'];

// See https://vue-i18n.intlify.dev/guide/advanced/typescript.html#global-resource-schema-type-definition
/* eslint-disable @typescript-eslint/no-empty-interface */
declare module 'vue-i18n' {
  // define the locale messages schema
  export interface DefineLocaleMessage extends MessageSchema {}

  // define the datetime format schema
  export interface DefineDateTimeFormat {}

  // define the number format schema
  export interface DefineNumberFormat {}
}
/* eslint-enable @typescript-eslint/no-empty-interface */

// Update translation locale based on LocalStorage
if (!LocalStorage.getItem('locale')) {
  try {
    LocalStorage.set('locale', 'fr');
  } catch (error) {
    // eslint-disable-next-line no-console
    console.log(error);
  }
}

const i18n = createI18n({
  locale: LocalStorage.getItem('locale') as string,
  legacy: false,
  warnHtmlMessage: false,
  messages,
});

// Update Quasar components translation
import(
  /* webpackInclude: /(fr|en-US)\.js$/ */
  `quasar/lang/${LocalStorage.getItem('locale')}`
).then((language):void => {
  Quasar.lang.set(language.default);
});

export default boot(({ app }):void => {
  // Set i18n instance on app
  app.use(i18n);
});

export { i18n };

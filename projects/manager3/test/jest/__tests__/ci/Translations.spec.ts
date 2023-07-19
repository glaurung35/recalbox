import fs from 'fs';
import util from 'util';

const R = require('ramda');

const readdir = util.promisify(fs.readdir);
const toKeysObject = (obj:object) => R.map(
  ([key, value]:[string, string]) => (typeof value === 'object' ? [key, toKeysObject(value)] : key),
  R.toPairs(obj),
);
const getKeys = (list:string, folder:string) => R.reduce(
  (acc:object, file:string) => R.assoc(
    file,
    toKeysObject(
      jest.requireActual(`../../../../src/i18n/${folder}/${file}`),
    ),
    acc,
  ),
  {},
  list,
);

describe('Locales tests', ():void => {
  let folders:string[] = [];
  let filteredFolders:string[] = [];
  let translations:object = {};
  let lnCodeRef = '';

  beforeEach(async ():Promise<void> => {
    folders = await readdir('src/i18n');
    filteredFolders = folders.filter((value: string): boolean => value !== 'index.ts');
  });

  it('I18n folder should contains at least 1 sub folder', async ():Promise<void> => {
    expect(filteredFolders.length).toBeGreaterThan(1);
  });

  it('Each translation folder should have the same translation file', async ():Promise<void> => {
    const translationsFiles:object = await Promise.all(
      R.map(
        (folder:string) => readdir(`src/i18n/${folder}`),
        filteredFolders,
      ),
    );
    translations = R.zipObj(filteredFolders, translationsFiles);
    // eslint-disable-next-line prefer-destructuring
    lnCodeRef = filteredFolders[1]; // fr-FR

    R.forEach((files:object):void => {
      expect(files).toStrictEqual(translations[lnCodeRef as keyof typeof translations]);
    }, translationsFiles);
  });

  it('All locales should have same keys', async ():Promise<void> => {
    const translationKeys = R.reduce(
      (
        acc:object,
        [lnCode, paths]:[string, string],
      ) => R.assoc(lnCode, getKeys(paths, lnCode), acc),
      {},
      R.toPairs(translations),
    );

    R.forEach((lnCode:string):void => {
      const filesKeys = R.toPairs(translationKeys[lnCode]);

      R.forEach(([file, keys]:[string, object]):void => {
        try {
          expect(keys).toStrictEqual(
            translationKeys[lnCodeRef][file],
          );
        } catch (e) {
          // eslint-disable-next-line no-console
          console.error(
            `Assertion failed on src/i18n/${lnCode}/${file}`,
          );
          throw e;
        }
      }, filesKeys);
    }, filteredFolders);
  });
});

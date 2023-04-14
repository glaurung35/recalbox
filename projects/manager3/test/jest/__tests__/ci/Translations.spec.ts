import fs from 'fs';
import util from 'util';

const R = require('ramda');

const readdir = util.promisify(fs.readdir);

const toKeysObject = (obj:object) => R.map(
  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  ([key, value]) => (typeof value === 'object' ? [key, toKeysObject(value)] : key),
  R.toPairs(obj),
);

// eslint-disable-next-line @typescript-eslint/ban-ts-comment
// @ts-ignore
const getKeys = (list, folder) => R.reduce(
// eslint-disable-next-line @typescript-eslint/ban-ts-comment
// @ts-ignore
  (acc, file) => R.assoc(
    file,
    toKeysObject(
      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore
      jest.requireActual(`../../../../src/i18n/${folder}/${file}`),
    ),
    acc,
  ),
  {},
  list,
);

// eslint-disable-next-line @typescript-eslint/ban-ts-comment
// @ts-ignore
describe('Locales tests', ():void => {
  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  it('all locales should have same keys', async ():Promise<void> => {
    const folders:string[] = await readdir('src/i18n');
    const filteredFolders:string[] = folders.filter((value:string):boolean => value !== 'index.ts');

    // we expect to have at least several languages
    // eslint-disable-next-line @typescript-eslint/ban-ts-comment
    // @ts-ignore
    expect(filteredFolders.length).toBeGreaterThan(1);

    // testing that each translation folder have the same translation file
    const translationsFiles = await Promise.all(
      R.map(
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        (folder) => readdir(`src/i18n/${folder}`),
        filteredFolders,
      ),
    );
    const translations = R.zipObj(filteredFolders, translationsFiles);
    const lnCodeRef:string = filteredFolders[1]; // fr-FR

    // eslint-disable-next-line @typescript-eslint/ban-ts-comment
    // @ts-ignore
    R.forEach((files):void => {
      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore
      expect(files).toStrictEqual(translations[lnCodeRef]);
    }, translationsFiles);

    // testing that each translation has the same keys
    const translationKeys = R.reduce(
      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore
      (acc, [lnCode, paths]) => R.assoc(lnCode, getKeys(paths, lnCode), acc),
      {},
      R.toPairs(translations),
    );

    // eslint-disable-next-line @typescript-eslint/ban-ts-comment
    // @ts-ignore
    R.forEach((lnCode):void => {
      const filesKeys = R.toPairs(translationKeys[lnCode]);

      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore
      R.forEach(([file, keys]):void => {
        try {
          // eslint-disable-next-line @typescript-eslint/ban-ts-comment
          // @ts-ignore
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

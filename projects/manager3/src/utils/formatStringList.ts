import { StringListObject } from 'stores/types/misc';

export const formatStringList = (stringListObject: StringListObject) => stringListObject.allowedStringList.map(
  (value, index) => (
    {
      value,
      label: stringListObject.displayableStringList[index],
    }
  ),
).sort((a, b) => a.label.localeCompare(b.label));

export interface BiosColumn {
  name: string;
  align: string;
  label: string;
  field: string;
  sortable: boolean;
  required?: boolean|undefined;
}

export interface GlobalMenuLink {
  title: string;
  icon: string;
  route: string;
  exact?: boolean;
}

export interface SimpleStringListObject {
  type: string;
  allowedStringList: string[];
}

export interface StringListObject extends SimpleStringListObject{
  displayableStringList: string[];
}

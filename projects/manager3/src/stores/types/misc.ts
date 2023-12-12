export interface BiosColumn {
  name: string;
  align: string;
  label: string;
  field: string;
  sortable: boolean;
  required?: boolean|undefined;
}

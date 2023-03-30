/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { boot } from 'quasar/wrappers';
import VueApexCharts from 'vue3-apexcharts';

export default boot(({ app }) => {
  // Set apexcharts instance on app
  app.use(VueApexCharts);
});

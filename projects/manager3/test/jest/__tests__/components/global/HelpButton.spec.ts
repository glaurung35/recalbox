import HelpButton from 'components/global/HelpButton.vue';
import { mount } from '@vue/test-utils';

describe('HelpButton', ():void => {
  test('Default : renders successfully', ():void => {
    const helpButton = mount(HelpButton, {
      propsData: {
        text: 'test',
      },
      global: {
        mocks: {
          $t: (text: string) => text,
        },
      },
    });
    expect(helpButton).toMatchSnapshot();
  });

  test('With warning icon : renders successfully', ():void => {
    const helpButton = mount(HelpButton, {
      propsData: {
        text: 'test',
        warning: true,
      },
      global: {
        mocks: {
          $t: (text: string) => text,
        },
      },
    });
    expect(helpButton).toMatchSnapshot();
  });

  test('For toggle : renders successfully', ():void => {
    const helpButton = mount(HelpButton, {
      propsData: {
        text: 'test',
        toggle: true,
      },
      global: {
        mocks: {
          $t: (text: string) => text,
        },
      },
    });
    expect(helpButton).toMatchSnapshot();
  });

  test('For toggle, with warning icon : renders successfully', ():void => {
    const helpButton = mount(HelpButton, {
      propsData: {
        text: 'test',
        toggle: true,
      },
      global: {
        mocks: {
          $t: (text: string) => text,
        },
      },
    });
    expect(helpButton).toMatchSnapshot();
  });
});

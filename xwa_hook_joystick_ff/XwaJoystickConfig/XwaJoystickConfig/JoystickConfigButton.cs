using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    public class JoystickConfigButton : INotifyPropertyChanged
    {
        private string controller;

        private string key;

        private int value;

        private bool isPressed;

        public event PropertyChangedEventHandler PropertyChanged;

        public JoystickConfigButton()
        {
        }

        public JoystickConfigButton(string controller, string key, int value)
        {
            this.controller = controller;
            this.key = key;
            this.value = value;
        }

        public string Controller
        {
            get
            {
                return this.controller;
            }

            set
            {
                if (value != this.controller)
                {
                    this.controller = value;
                    this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Controller)));
                }
            }
        }

        public string Key
        {
            get
            {
                return this.key;
            }

            set
            {
                if (value != this.key)
                {
                    this.key = value;
                    this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Key)));
                }
            }
        }

        public int Value
        {
            get
            {
                return this.value;
            }

            set
            {
                if (value != this.value)
                {
                    this.value = value;
                    this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Value)));
                }
            }
        }

        public bool IsPressed
        {
            get
            {
                return this.isPressed;
            }

            set
            {
                if (value != this.isPressed)
                {
                    this.isPressed = value;
                    this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(IsPressed)));
                }
            }
        }
    }
}

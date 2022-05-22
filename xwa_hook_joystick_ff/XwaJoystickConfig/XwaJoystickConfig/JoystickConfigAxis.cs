using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    public class JoystickConfigAxis : INotifyPropertyChanged
    {
        private string controllerName;

        private string controller;

        private string axis;

        private int position;

        public event PropertyChangedEventHandler PropertyChanged;

        public JoystickConfigAxis()
        {
        }

        public JoystickConfigAxis(string controller, string controllerName, string axis)
        {
            this.controllerName = controllerName;
            this.controller = controller;
            this.axis = axis;
        }

        public string ControllerName
        {
            get
            {
                return this.controllerName;
            }

            set
            {
                if (value != this.controllerName)
                {
                    this.controllerName = value;
                    this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ControllerName)));
                }
            }
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

        public string Axis
        {
            get
            {
                return this.axis;
            }

            set
            {
                if (value != this.axis)
                {
                    this.axis = value;
                    this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Axis)));
                }
            }
        }

        public int Position
        {
            get
            {
                return this.position;
            }

            set
            {
                if (value != this.position)
                {
                    this.position = value;
                    this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Position)));
                }
            }
        }
    }
}
